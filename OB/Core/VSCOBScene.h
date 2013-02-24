
#ifndef _VSC_OB_SCENE_H_
#define _VSC_OB_SCENE_H_

#include "VSCOB.h"
#include "VSCOBDisplay.h"
#include "VSCBroadcaster.h"
#include "VSCListener.h"

#include "OgreBulletDynamics.h"

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

        class Scene : public Ogre::FrameListener, public VSC::Broadcaster, public boost::enable_shared_from_this<Scene>
        {
            
        public:
            
            friend class Application;
            
            /*
             *  Pointer typdefs
             */
            
            typedef boost::shared_ptr<Scene>    SPtr;
            typedef boost::weak_ptr<Scene>      WPtr; // Weak pointers are needed for Element and Factory objects
            
            /*
             *  Container typedefs
             */
            
            typedef std::map<std::string, Ogre::Light*> LightMap;
            
            class Bridge {
                
            public:
                
                friend class Scene;
            
                typedef boost::shared_ptr<Bridge>   SPtr;
                
                virtual ~Bridge();
                Scene::SPtr getScene(void) {return mScene.lock();}
                
            protected:
                
                Bridge(Scene::SPtr scene) { mScene = Scene::WPtr(scene); }
                
            private:
                
                Scene::WPtr mScene;
                
            };
            
            friend class Bridge;
            
            /*
             *  Enum for on/off scene settings
             */
            
            enum Setting {
                SettingNone = 0,
                SettingPaused,
                SettingDrawWireFrame,
                SettingDrawAABB,
                SettingDrawText,
                SettingDrawFeaturesText,
                SettingDrawContactPoints,
                SettingBulletLCPIEnabled,
                SettingCCDEnabled,
                SettingNoDeactivation,
                SettingProfileTimings,
                SettingSatComparisonEnabled
            };
            
            /*
             *  The scene listener is used by elements which want to be informed
             *  of scene events. This is primarily for UI updates put can potentially
             *  be used for other stuff.
             */
            
            class Listener : public VSC::Listener
            {
                
            public:
                
                typedef boost::shared_ptr<Listener>    SPtr;
                typedef boost::weak_ptr<Listener>      WPtr;
                
                virtual ~Listener() {}
                
                virtual void sceneRegisteredElement(Scene::SPtr scene, Element_SPtr element) {}
                virtual void sceneChangedSetting(Scene::SPtr scene, Setting setting, bool value) {}
                virtual void sceneWasRendered(Scene::SPtr scene) {}
                
            };
            

            virtual ~Scene() { /* shutdown(); // calling shutdown in destructor causes problems */  };
            
            /**--------------------------------------------------------------
             *  Factory and Element weak_ptr container
             */
            
            ElementFactory_SPtr getElementFactory(void) {return mElementFactory;}
            void setElementFactory(ElementFactory_SPtr factory) {mElementFactory = factory;}
            
            //void removeElement(Element_SPtr element);
            //void removeAllElements(void);

            /**--------------------------------------------------------------
             *  MISC Info
             */
            const Ogre::String getName() {return mName;}

            /**--------------------------------------------------------------
             *  Ogre Frame Listener Forwarded messages from Application 
             */
            
            virtual bool frameStarted(const Ogre::FrameEvent& evt);
            virtual bool frameEnded(const Ogre::FrameEvent& evt);

            /**--------------------------------------------------------------
             *  Dynamic actions and checks
             */
            
            bool checkIfEnoughPlaceToAddObjectForDisplay(Display::SPtr display, float maxDist);
            
            Element_SPtr getElementAtDisplayCoordinate(Display::SPtr display, const Ogre::Vector2& p,
                                                        Ogre::Vector3 &ip, Ogre::Ray &r);
            
            OgreBulletCollisions::DebugLines* getDebugLines() {return mDebugLines;}
            
            /**--------------------------------------------------------------
             *  Ogre Getters
             */
            
            OgreBulletDynamics::DynamicsWorld* getDynamicsWorld(void) {return mDynamicsWorld;}
            Ogre::SceneManager* getSceneManager(void) {return mSceneManager;}
            LightMap& getLightMap(void) {return mLightMap;}
            
            
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
            void destroyElement(Element_SPtr element);
            
            /*
             *  Get element using the inner rigid body.
             */
            Element_SPtr getElementWithRigidBody(OgreBulletDynamics::RigidBody* rigidBody);
            
            /*
             *  Provide access the elements which have been created by the factory.
             */
            Elements& getElements() {return mElements;}
            
            /**--------------------------------------------------------------
             *  Collisions
             */
            
            CollisionDetector_SPtr getCollisionDetector(void) {return mCollisionDetector;}
            void setCollisionDetector(CollisionDetector_SPtr detector);
            
            /**--------------------------------------------------------------
             *  Settings
             */
            
            void toggleSetting(Setting setting);
            void setSetting(Setting setting, bool on);
            bool getSetting(Setting setting);
            
            /*
             *  Step
             */
            
            void doOneStep() {mDoOneStep = true;} // steps through simulation/dynamics useful when paused
            
        protected:
            
            typedef std::vector<Listener::WPtr> Listeners;
            
            Scene();
            
            void init();
            void shutdown();
            
            virtual void internalInit(void); // for subclasses to provide additional init steps (called by init())
            virtual void internalShutdown(void); // for subclasses to provide additional shutdown steps (called by shutdown())
            
            virtual void initWorld (const Ogre::Vector3& gravityVector =
                                    Ogre::Vector3(0,-9.81,0),
                                    const Ogre::AxisAlignedBox& bounds =
                                    Ogre::AxisAlignedBox (Ogre::Vector3(-10000, -10000, -10000),
                                                          Ogre::Vector3(10000,  10000,  10000)));
            
            virtual void setupFactory();
            virtual void setupLights();
            
            void setInfoText();
            void setDebugText(const Ogre::String &debugText) {mDebugText = debugText;}
            void setName(Ogre::String name) {mName = name;}
            
            /*
             *  Protected setters for world setup 
             */
            
            void setDynamicsWorld(OgreBulletDynamics::DynamicsWorld* world) {mDynamicsWorld = world;}
            
            virtual void createSceneManager(void);
            virtual void createDebugLines(void);
            
            /*
             *  Broadcaster listener check
             */
            
            virtual bool checkListener(VSC::Listener::SPtr listener);
            
        public:
            /*
             *  Registering an element will add a shared_ptr to the element to the factory's private container
             *  and will add a weak_ptr to the scenes private container
             */
            void registerElement(Element_SPtr element, std::string name, int identifier);
            
        private:
            
            Bridge::SPtr                            mBridge;
            
            ElementFactory_SPtr                    mElementFactory;
            Elements                                mElements;
            CollisionDetector_SPtr                 mCollisionDetector;
            
            OgreBulletDynamics::DynamicsWorld*      mDynamicsWorld;
            Ogre::SceneManager*                     mSceneManager;
            LightMap                                mLightMap;
            Ogre::ShadowTechnique                   mCurrentShadowTechnique;
            
            OgreBulletCollisions::DebugLines*       mDebugLines;
            Ogre::String                            mDebugText;
            Ogre::String                            mName;
            
            static size_t                           mNumEntitiesInstanced;
            
            static const bool                       mTraceFrame = false;
            static const bool                       mTraceUI = false;
            
            bool mDoOneStep;
            
            typedef std::map<Setting, bool> SettingValueMap;
            
            SettingValueMap mSettingValueMap;
            
        };
        
        typedef std::vector<Scene::SPtr> Scenes;
        
        
    }
}

#endif //_VSC_OB_SCENE_H_

