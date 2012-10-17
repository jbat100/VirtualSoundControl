
#ifndef _VSC_OGRE_BULLET_SCENE_H_
#define _VSC_OGRE_BULLET_SCENE_H_

#include "OgreBulletDynamics.h"
#include "VSCOBInputListener.h"
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
        
        class Application;

        class Scene : public boost::enable_shared_from_this<Scene>
        {
            
        private:
            
            typedef boost::shared_ptr<Application>  Application_SPtr;
            typedef boost::weak_ptr<Application>    Application_WPtr;
            
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
            
            class ElementFactory;
            
            class Element
            {
                
            public:
                
                friend class Scene;
                friend class ElementFactory;
                
                typedef boost::shared_ptr<Element>    SPtr;   // Smart pointers are only used to reference Elements by Scene::Factory
                typedef boost::weak_ptr<Element>      WPtr;   // Weak pointers are used everywhere else
                
                Element(Scene::WPtr scene, OgreBulletDynamics::RigidBody* rigidBody) :
                mScene(scene), mRigidBody(rigidBody), mSilentCollisions(false) {}
                
                virtual ~Element() { destroy(); }
                
                /*
                 *  No public setter, cannot change the element internals once it is constructed. This
                 *  deliberate design decision.
                 */
                
                Scene::WPtr                             getScene(void) const {return mScene;};
                OgreBulletDynamics::RigidBody*          getRigidBody(void) const {return mRigidBody;}
                
                bool                                    silentCollisions() const {return mSilentCollisions;}
                void                                    setSilentCollisions(bool silent) {mSilentCollisions = silent;}
                
                std::string                             getName(void) const  {return mName;}
                int                                     getIdentifier(void) const {return mIdentifier;}
                
            protected:
                
                /*
                 *  Destroy should be overridden to perform all necessary operations to remove the element from the Scene
                 *  and all associated Ogre/Bullet stuff which it is used by. However subclasses should call the base implementation
                 *  which removes the rigid body from the OgreBulletDynamics::World.
                 */
                
                virtual void                            destroy(void);
                
                void                                    setName(std::string n) {mName = n;}
                void                                    setIdentifier(int i) {mIdentifier = i;}
                
                
            private:
                
                Scene::WPtr                             mScene;
                OgreBulletDynamics::RigidBody*          mRigidBody;
                bool                                    mSilentCollisions;
                
                std::string                             mName;
                int                                     mIdentifier;
                
            };
            
            typedef std::vector<Element::SPtr> Elements;
            
            /*
             *  Scene::Factory is used to generate Scene::Element and keep a shared_ptr to them. Like Elemet, it is meant 
             *  to be subclassed to be useful.
             */
            
            class ElementFactory
            {
            public:
                
                typedef boost::shared_ptr<ElementFactory> SPtr;
                
                ElementFactory(Scene::WPtr scene) : mScene(scene) {}
                virtual ~ElementFactory() { }
                
                Scene::WPtr getScene(void) {return mScene;}; // no public setter, cannot change the element scene.
                
                /*
                 *  Abstract API for adding the stuff to the world. Used directly by VSC::OB::Scene 
                 */
                virtual void addGround(void) = 0;
                
            protected:
                
                
            private:
                
                Scene::WPtr mScene;
                
            };
            
            /*
             *  Collisions, used for tracking and informing Collision listeners
             */
            
            class CollisionDetector; // forward declaration allowing friend status to be declared
            
            class Collision
            {
                
            public:
                
                friend class Scene;
                friend class CollisionDetector;
                
                typedef boost::shared_ptr<Collision>        SPtr;
                typedef boost::weak_ptr<Collision>          WPtr;
                
                //typedef std::vector<btPersistentManifold*>  PersistentManifolds;
                
                enum State {
                    StateNone = 0,
                    StateCreated,
                    StateClose,
                    StateOngoing,
                    StateEnded,
                    StateInvalid
                };
                
                Collision() {}
                
                Scene::SPtr getScene(void) {return mScene.lock();}
                
                Scene::Element::SPtr getFirstElement(void) const {return mFirstElement.lock();}
                Scene::Element::SPtr getSecondElement(void) const {return mSecondElement.lock();}
                
                State getState(void) {return mState;}
                
                //const PersistentManifolds& getPersistentManifolds(void) {return mPersistentManifolds;}
                
                btPersistentManifold* getPersistentManifold(void) {return mPersistentManifold;}
                
            protected:
                
                void setScene(Scene::WPtr scene);
                
                void setFirstElement(Element::SPtr element) {mFirstElement = Element::WPtr(element);}
                void setSecondElement(Element::SPtr element) {mSecondElement = Element::WPtr(element);}
                
                void setState(State state) {mState = state;}
                
                /*
                 *  On each physics update the manifolds for the object will be cleared and repopulated
                 *  with the current dynamics world manifolds.
                 */
                
                //void addPersistentManifold(btPersistentManifold* manifold);
                //void clearPersistentManifolds();
                
                void setPersistentManifold(btPersistentManifold* manifold) {mPersistentManifold = manifold;}
                
                /*  
                 *  Used by friend class CollisionDetector once the collision is no longer valid. Smart pointers
                 *  to the collision object might keep it alive but it's internals will be nulled. Listeners
                 *  should purge their smart pointers to invalidated objects on every callback to allow memory
                 *  to be cleaned up.
                 */
                
                void invalidate();
                
            private:
                
                Scene::WPtr             mScene;
                
                Element::WPtr           mFirstElement;
                Element::WPtr           mSecondElement;
                
                State                   mState;
                
                btPersistentManifold*   mPersistentManifold;
                
                //PersistentManifolds     mPersistentManifolds;
                
            };
            
            
            
            typedef std::vector<Collision::SPtr>            Collisions;
    
            class CollisionListener
            {
                
            public:
                
                typedef boost::shared_ptr<CollisionListener>    SPtr;
                typedef boost::weak_ptr<CollisionListener>      WPtr;
                
                virtual void collisionProspectDetected(Scene::Collision::SPtr collision) {}
                virtual void collisionProspectUpdated(Scene::Collision::SPtr collision) {}
                virtual void collisionProspectEnded(Scene::Collision::SPtr collision) {}
                
                virtual void collisionDetected(Scene::Collision::SPtr collision) {}
                virtual void collisionUpdated(Scene::Collision::SPtr collision) {}
                virtual void collisionEnded(Scene::Collision::SPtr collision) {}
                
            };
            
            typedef std::vector<CollisionListener::SPtr>    Listeners;
            
            class CollisionDetector
            {
                
            public:
                
                typedef boost::shared_ptr<CollisionDetector>    SPtr;
                typedef boost::weak_ptr<CollisionDetector>      WPtr;
                
                CollisionDetector(Scene::WPtr scene) : mScene(scene), mDistanceThreshold(0.1) {}
                
                const Collisions&   getCollisions(void) {return mCollisions;}
                const Listeners&    getListeners(void) {return mListeners;}
                
                void addListener(CollisionListener::SPtr listener);
                void removeListener(CollisionListener::SPtr listener);
                
                Collisions getCollisionsForElementPair(Scene::Element::SPtr first, Scene::Element::SPtr second);
                
                Scene::WPtr getScene(void) {return mScene;}
                
                virtual void updateCollisions();
                
            protected:
                
                void addCollision(Collision::SPtr collision);
                void removeCollision(Collision::SPtr collision);
                
                Collision::SPtr getCollisionsForPersistentManifold(btPersistentManifold* manifold);
                
            private:
                
                Collisions      mCollisions;
                Listeners       mListeners;
                
                Scene::WPtr     mScene;
                
                Ogre::Real      mDistanceThreshold;
                
            };

            /**--------------------------------------------------------------
             *  Constructor/Destructor/Initialization 
             */
            Scene();
            virtual ~Scene(){ /* shutdown(); // calling shutdown in destructor causes problems */  };
            
            virtual void init(Application_SPtr application);

            /**--------------------------------------------------------------
             *  Setup/Teardown 
             */
            virtual void shutdown();
            
            /**--------------------------------------------------------------
             *  Application
             */
            Application_SPtr getApplication(void) {return mApplication.lock();}
            
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
            Element::SPtr getElementAtViewportCoordinate(const Ogre::Viewport* v, Ogre::Vector2& p, Ogre::Vector3 &ip, Ogre::Ray &r);
            OgreBulletCollisions::DebugLines* getDebugRayLines();
            
            /**--------------------------------------------------------------
             *  Ogre Getters
             */
            
            OgreBulletDynamics::DynamicsWorld* getDynamicsWorld(void) {return mWorld;}
            Ogre::SceneManager* getSceneManager(void) {return mSceneManager;}
            LightMap& getLightMap(void) {return mLightMap;}
            
            /**--------------------------------------------------------------
             *  Stats Getters
             */
            
            const StatsMap& getUpdatedStatsMap(void);
            
            /**--------------------------------------------------------------
             * Elements
             */
            
            /*
             *  Reset should destroy all shared_ptr references to the Elements that the factory has created
             */
            void destroyAllElements(void);
            
            /*
             *  destroyElement() should remove the element from the scene, simply by erasing it from the
             *  internal shared_ptr container (which calls the Element's destructor and therefore it's destroy()
             *  method, which should in theory do its cleaning up duties).
             */
            void destroyElement(Scene::Element::SPtr element);
            
            /*
             *  Get element using the inner rigid body.
             */
            Element::SPtr getElementWithRigidBody(OgreBulletDynamics::RigidBody* rigidBody);
            
            /*
             *  Provide access the elements which have been created by the factory.
             */
            Elements& elements() {return mElements;}
            
            /*
             *  Registering an element will add a shared_ptr to the element to the factory's private container
             *  and will add a weak_ptr to the scenes private container
             */
            void registerElement(Scene::Element::SPtr element, std::string name, int identifier);
            
            /**--------------------------------------------------------------
             *  Collisions
             */
            
            CollisionDetector::SPtr getCollisionDetector(void) {return mCollisionDetector;}
            void setCollisionDetector(CollisionDetector::SPtr detector);
            
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

            
            
            virtual void createSceneManager(void);
            
        private:
            
            Application_WPtr                        mApplication;
            
            ElementFactory::SPtr                    mElementFactory;
            Elements                                mElements;
            CollisionDetector::SPtr                 mCollisionDetector;
            
            OgreBulletDynamics::DynamicsWorld*      mWorld;
            Ogre::RenderWindow*                     mWindow;
            Ogre::Root*                             mRoot;
            Ogre::SceneManager*                     mSceneManager;
            Ogre::Camera*                           mCamera;
            LightMap                                mLightMap;
            Ogre::ShadowTechnique                   mCurrentShadowTechnique;
            
            StatsMap                                mStatsMap;
            
            OgreBulletCollisions::DebugLines*       mDebugRayLine;
            Ogre::String                            mDebugText;
            Ogre::String                            mName;
            
            static size_t                           mNumEntitiesInstanced;
            
            static const bool                       mTraceFrame = false;
            static const bool                       mTraceUI = false;
            
            bool mDoOneStep;
            bool mPaused;
            
            bool mDrawWireFrame;
            bool mDrawAabb;
            bool mDrawFeaturesText;
            bool mDrawContactPoints;
            bool mDrawText;
            
            bool mDisableBulletLCP;
            bool mEnableCCD;
            
            /*
             *  Useful?
             */
            
            bool mNoDeactivation;
            bool mNoHelpText;
            bool mProfileTimings;
            bool mEnableSatComparison;
            
        };
        
        std::ostream& operator << (std::ostream& stream, const Scene::Collision& collision);
        
        std::ostream& operator << (std::ostream& stream, const Scene::Element& element);
        
    }
}

#endif //_VSC_OGRE_BULLET_SCENE_H_
