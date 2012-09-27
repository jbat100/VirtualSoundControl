
#ifndef _VSC_OGRE_BULLET_SCENE_H_
#define _VSC_OGRE_BULLET_SCENE_H_

#include "OgreBulletDynamics.h"
#include "VSCOBInputListener.h"
#include "VSCOBBetaGUIListener.h"
#include "VSCOBKeyboardAction.h"
#include "VSCOBKeyBindings.h"
#include "VSCOBCameraController.h"
#include "OIS.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp> 

#include <map>
#include <string>


namespace VSC {
    
    namespace OB {
        
        /*
         *  A Scene is where the action takes place, it contains the ogre/bullet stuff which is used 
         *  to represent it. It offers APIs to create and control elements (through its factory). It does
         *  not however respond directly to user input. Use VSC::OB::SceneController for that.
         */

        class Scene : public boost::enable_shared_from_this<Scene>
        {
            
        public:
            
            /*
             *  Pointer typdefs
             */
            
            typedef boost::shared_ptr<Scene>    SPtr;
            typedef boost::weak_ptr<Scene>      WPtr; // Weak pointers are needed for Element and Factory objects
            
            /*
             *  Container typedefs
             */
            
            typedef std::map<std::string, Ogre::Light*>     LightMap;
            typedef std::map<std::string, std::string>      StatsMap;
            
            /*
             *  A Scene::Element represents an object in the scene, dynamic or static.
             *  It is generated by Scene::Factory. It should be subclassed to associate an ogre
             *  representation in the scene.
             */
            
            class Element
            {
                
            public:
                
                typedef boost::shared_ptr<Element>    SPtr;   // Smart pointers are only used to reference Elements by Scene::Factory
                typedef boost::weak_ptr<Element>      WPtr;   // Weak pointers are used everywhere else
                
                Element(Scene::WPtr scene, OgreBulletDynamics::RigidBody* rigidBody) : mScene(scene), mRigidBody(rigidBody) {}
                virtual ~Element() { destroy(); }
                
                /*
                 *  No public setter, cannot change the element internals once it is constructed. This
                 *  deliberate design decision.
                 */
                
                Scene::WPtr                             getScene(void) {return mScene;};
                OgreBulletDynamics::RigidBody*          getRigidBody(void) {return mRigidBody;}
                
                /*
                 *  Destroy should be overridden to perform all necessary operations to remove the element from the Scene
                 *  and all associated Ogre/Bullet stuff which it is used by. However subclasses should call the base implementation
                 *  which removes the rigid body from the OgreBulletDynamics::World.
                 */
                
                virtual void destroy(void);
                
            protected:
                
                
            private:
                
                Scene::WPtr                             mScene;
                OgreBulletDynamics::RigidBody*          mRigidBody;
                
            };
            
            /*
             *  Scene::Factory is used to generate Scene::Element and keep a shared_ptr to them. Like Elemet, it is meant 
             *  to be subclassed to be useful.
             */
            
            class ElementFactory
            {
            public:
                
                typedef boost::shared_ptr<ElementFactory> SPtr;
                typedef std::vector<Scene::Element::SPtr> SElements;
                
                ElementFactory(Scene::WPtr scene) : mScene(scene) {}
                virtual ~ElementFactory() { reset(); }
                
                Scene::WPtr getScene(void) {return mScene;}; // no public setter, cannot change the element scene.
                
                /*
                 *  Reset should destroy all shared_ptr references to the Elements that the factory has created
                 */
                void reset(void);
                
                /*
                 *  destroyElement() should remove the element from the scene, simply by erasing it from the 
                 *  internal shared_ptr container (which calls the Element's destructor and therefore it's destroy()
                 *  method, which should in theory do its cleaning up duties).
                 */
                void destroyElement(Scene::Element::WPtr element);
                
                /*
                 *  Get element using the inner rigid body.
                 */
                Element::WPtr elementWithRigidBody(OgreBulletDynamics::RigidBody* rigidBody);
                
                /*
                 *  Provide access the elements which have been created by the factory.
                 */
                SElements& elements() {return mSElements;}
                
                /*
                 *  Abstract API for adding the stuff to the world. Used directly by VSC::OB::Scene 
                 */
                virtual void addGround(void) = 0;
                
            protected:
                
                /*
                 *  Registering an element will add a shared_ptr to the element to the factory's private container
                 *  and will add a weak_ptr to the scenes private container
                 */
                void registerElement(Scene::Element::SPtr element);
                
            private:
                
                Scene::WPtr mScene;
                
                SElements mSElements;
            };
            
            typedef std::deque<Element::WPtr> Elements;
            

            /**--------------------------------------------------------------
             *  Constructor/Destructor/Initialization 
             */
            Scene();
            virtual ~Scene(){};
            virtual void init(Ogre::Root *root, Ogre::RenderWindow *win);

            /**--------------------------------------------------------------
             *  Setup/Teardown 
             */
            virtual void shutdown();
            
            /**--------------------------------------------------------------
             *  Camera
             */
            bool resetCameraAspectRatio(void);
            
            /**--------------------------------------------------------------
             *  Factory and Element weak_ptr container
             */
            
            ElementFactory::SPtr getElementFactory(void) {return mElementFactory;}
            void setElementFactory(ElementFactory::SPtr factory) {mElementFactory = factory;}

            /**--------------------------------------------------------------
             *  MISC Info
             */
            const Ogre::String getName() {return mName;}

            /**--------------------------------------------------------------
             *  Ogre Frame Listener Forwarded messages from Application 
             */
            virtual bool frameStarted(Ogre::Real elapsedTime);
            virtual bool frameEnded(Ogre::Real elapsedTime);

            /**--------------------------------------------------------------
             *  Dynamic actions and checks
             */
            bool checkIfEnoughPlaceToAddObject(float maxDist);
            Element::WPtr getElementAtViewportCoordinate(const Ogre::Viewport* v, Ogre::Vector2& p, Ogre::Vector3 &ip, Ogre::Ray &r);
            OgreBulletCollisions::DebugLines* getDebugRayLines();
            
            /**--------------------------------------------------------------
             *  Ogre Getters
             */
            
            OgreBulletDynamics::DynamicsWorld* getDynamicsWorld(void) {return mWorld;}
            Ogre::RenderWindow* getRenderWindow(void) {return mWindow;}
            Ogre::Root* getRoot(void) {return mRoot;}
            Ogre::SceneManager* getSceneManager(void) {return mSceneMgr;}
            Ogre::Camera* getCamera(void) {return mCamera;}
            LightMap& getLightMap(void) {return mLightMap;}
            
            /**--------------------------------------------------------------
             *  Stats Getters
             */
            
            const StatsMap& getUpdatedStatsMap(void);
            
            /**--------------------------------------------------------------
             *  Other Setters/Getters
             */
            
            void doOneStep() {mDoOneStep = true;} // steps through simulation/dynamics useful when paused
            
            bool isPaused(void) {return mPaused;}
            void pause(bool p) {mPaused = p;}
            void togglePause(void) {mPaused = !mPaused;}
            
            bool drawingWireFrame(void) {return mDrawWireFrame;}
            void drawWireFrame(bool draw);
            void toggleDrawWireFrame(void) {drawWireFrame(!mDrawWireFrame);}
            
            bool drawingAabb(void) {return mDrawAabb;}
            void drawAabb(bool draw);
            void toggleDrawAabb(void) {mDrawAabb = !mDrawAabb;}
            
            bool drawingText(void) {return mDrawText;}
            void drawText(bool draw);
            void toggleDrawText(void) {mDrawText = !mDrawText;}
            
            bool drawingFeaturesText(void) {return mDrawFeaturesText;}
            void drawFeaturesText(bool draw);
            void toggleDrawFeaturesText(void) {mDrawFeaturesText = !mDrawFeaturesText;}
            
            bool drawingContactPoints(void) {return mDrawContactPoints;}
            void drawContactPoints(bool draw);
            void toggleDrawContactPoints(void) {mDrawContactPoints = !mDrawContactPoints;}
            
            bool bulletLCPIsEnabled(void) {return !mDisableBulletLCP;}
            void enableBulletLCP(bool enable);
            void toggleBulletLCP(void) {mDisableBulletLCP = !mDisableBulletLCP;}
            
            bool ccdIsEnabled(void) {return mEnableCCD;}
            void enableCCD(bool enable);
            void toggleCCD(void) {mEnableCCD = !mEnableCCD;}
            
        protected:
            
            virtual void initWorld (const Ogre::Vector3& gravityVector = Ogre::Vector3 (0,-9.81,0),
                                    const Ogre::AxisAlignedBox& bounds = Ogre::AxisAlignedBox (Ogre::Vector3 (-10000, -10000, -10000),
                                                                                               Ogre::Vector3 (10000,  10000,  10000)));
            
            virtual void setupFactory();
            virtual void setupLights();
            
            void setInfoText();
            void setDebugText(const Ogre::String &debugText) {mDebugText = debugText;}
            void getDebugLines();
            void setName(Ogre::String name) {mName = name;}
            
            /*
             *  Protected setters for world setup 
             */
            
            void setDynamicsWorld(OgreBulletDynamics::DynamicsWorld* world) {mWorld = world;}
            void setRenderWindow(Ogre::RenderWindow* window) {mWindow = window;}
            void setRoot(Ogre::Root* root) {mRoot = root;}
            void setSceneManager(Ogre::SceneManager* manager) {mSceneMgr = manager;}
            void setCamera(Ogre::Camera* camera) {mCamera = camera;}
            
        private:
            
            ElementFactory::SPtr                    mElementFactory;
            
            OgreBulletDynamics::DynamicsWorld       *mWorld;
            Ogre::RenderWindow                      *mWindow;
            Ogre::Root                              *mRoot;
            Ogre::SceneManager                      *mSceneMgr;
            Ogre::Camera                            *mCamera;
            LightMap                                mLightMap;
            Ogre::ShadowTechnique                   mCurrentShadowTechnique;
            
            StatsMap                                mStatsMap;
            
            bool mDoOneStep;
            bool mPaused;
            
            bool mDrawWireFrame;
            bool mDrawAabb;
            bool mDrawFeaturesText;
            bool mDrawContactPoints;
            bool mDrawText;
            
            bool mDisableBulletLCP;
            bool mEnableCCD;
            
            OgreBulletCollisions::DebugLines    *mDebugRayLine;
            Ogre::String                        mDebugText;
            Ogre::String                        mName;
            
            static size_t mNumEntitiesInstanced;
            
            static const bool mTraceFrame = false;
            static const bool mTraceUI = true;
            
            /*
             *  Useful?
             */
            
            bool mNoDeactivation;
            bool mNoHelpText;
            bool mProfileTimings;
            bool mEnableSatComparison;
            
        };
        
    }
}

#endif //_VSC_OGRE_BULLET_SCENE_H_

