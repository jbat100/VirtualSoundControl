
#ifndef _VSC_OGRE_BULLET_SCENE_H_
#define _VSC_OGRE_BULLET_SCENE_H_

#include "VSCOBDisplay.h"
#include "VSCBroadcaster.h"
#include "VSCListener.h"

#include "OgreBulletDynamics.h"
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

        class Scene : public Ogre::FrameListener, public VSC::Broadcaster, public boost::enable_shared_from_this<Scene>
        {
            
        private:
            
            typedef boost::shared_ptr<Application>  Application_SPtr;
            typedef boost::weak_ptr<Application>    Application_WPtr;
            
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
                mScene(scene), mRigidBody(rigidBody), mSilentCollisions(false), mImmobilized(false) {}
                
                virtual ~Element() { destroy(); }
                
                /*
                 *  No public setter, cannot change the element internals once it is constructed. This
                 *  deliberate design decision.
                 */
                
                Scene::SPtr getScene(void) const {return mScene.lock();};
                OgreBulletDynamics::RigidBody* getRigidBody(void) const {return mRigidBody;}
                
                bool silentCollisions() const {return mSilentCollisions;}
                void setSilentCollisions(bool silent) {mSilentCollisions = silent;}
                
                std::string getName(void) const  {return mName;}
                int getIdentifier(void) const {return mIdentifier;}
                
                bool isImmobilized(void);
                void setImmobilized(bool immobilized);
                
                void setMass(Float mass);
                Float getMass(void);
                
            protected:
                
                /*
                 *  Destroy should be overridden to perform all necessary operations to remove the element from the Scene
                 *  and all associated Ogre/Bullet stuff which it is used by. However subclasses should call the base implementation
                 *  which removes the rigid body from the OgreBulletDynamics::World.
                 */
                
                virtual void destroy(void);
                
                void setName(std::string n) {mName = n;}
                void setIdentifier(int i) {mIdentifier = i;}
                
                
            private:
                
                Scene::WPtr                             mScene;
                OgreBulletDynamics::RigidBody*          mRigidBody;
                bool                                    mSilentCollisions;
                bool                                    mImmobilized;
                Float                                   mMass;
                
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
                
                Scene::SPtr getScene(void) {return mScene.lock();}; // no public setter, cannot change the element scene.
                
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
            
            typedef std::vector<Collision::SPtr> Collisions;
    
            
            class CollisionDetector : public VSC::Broadcaster
            {
                
            public:
                
                class Listener : public VSC::Listener {
                    
                public:
                    
                    typedef boost::shared_ptr<Listener>    SPtr;
                    typedef boost::weak_ptr<Listener>      WPtr;
                    
                    virtual void collisionProspectDetected(Scene::Collision::SPtr collision) {}
                    virtual void collisionProspectUpdated(Scene::Collision::SPtr collision) {}
                    virtual void collisionProspectEnded(Scene::Collision::SPtr collision) {}
                    
                    virtual void collisionDetected(Scene::Collision::SPtr collision) {}
                    virtual void collisionUpdated(Scene::Collision::SPtr collision) {}
                    virtual void collisionEnded(Scene::Collision::SPtr collision) {}
                    
                };
                
                typedef boost::shared_ptr<CollisionDetector>    SPtr;
                typedef boost::weak_ptr<CollisionDetector>      WPtr;
                
                CollisionDetector(Scene::SPtr scene) : mScene(Scene::WPtr(scene)), mDistanceThreshold(0.1) {}
                
                const Collisions&   getCollisions(void) {return mCollisions;}
                Collisions getCollisionsForElementPair(Scene::Element::SPtr first, Scene::Element::SPtr second);
                
                Scene::SPtr getScene(void) {return mScene.lock();}
                
                virtual void updateCollisions();
                
            protected:
                
                void addCollision(Collision::SPtr collision);
                void removeCollision(Collision::SPtr collision);
                
                Collision::SPtr getCollisionsForPersistentManifold(btPersistentManifold* manifold);
                
                virtual bool checkListener(VSC::Listener::SPtr listener);
                
            private:
                
                Collisions      mCollisions;
                
                Scene::WPtr     mScene;
                
                Ogre::Real      mDistanceThreshold;
                
            };
            
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
                
                virtual void sceneRegisteredElement(Scene::SPtr scene, Scene::Element::SPtr element) {}
                virtual void sceneChangedSetting(Scene::SPtr scene, Setting setting, bool value) {}
                virtual void sceneWasRendered(Scene::SPtr scene) {}
                
            };
            

            virtual ~Scene() { /* shutdown(); // calling shutdown in destructor causes problems */  };
            
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
            
            virtual bool frameStarted(const Ogre::FrameEvent& evt);
            virtual bool frameEnded(const Ogre::FrameEvent& evt);

            /**--------------------------------------------------------------
             *  Dynamic actions and checks
             */
            
            bool checkIfEnoughPlaceToAddObjectForDisplay(Display::SPtr display, float maxDist);
            
            Element::SPtr getElementAtDisplayCoordinate(Display::SPtr display, const Ogre::Vector2& p,
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
            void destroyElement(Scene::Element::SPtr element);
            
            /*
             *  Get element using the inner rigid body.
             */
            Element::SPtr getElementWithRigidBody(OgreBulletDynamics::RigidBody* rigidBody);
            
            /*
             *  Provide access the elements which have been created by the factory.
             */
            Elements& getElements() {return mElements;}
            
            /**--------------------------------------------------------------
             *  Collisions
             */
            
            CollisionDetector::SPtr getCollisionDetector(void) {return mCollisionDetector;}
            void setCollisionDetector(CollisionDetector::SPtr detector);
            
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
            void registerElement(Scene::Element::SPtr element, std::string name, int identifier);
            
        private:
            
            Bridge::SPtr                            mBridge;
            
            ElementFactory::SPtr                    mElementFactory;
            Elements                                mElements;
            CollisionDetector::SPtr                 mCollisionDetector;
            
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
        
        std::ostream& operator << (std::ostream& stream, const Scene::Collision& collision);
        
        std::ostream& operator << (std::ostream& stream, const Scene::Element& element);
        
    }
}

#endif //_VSC_OGRE_BULLET_SCENE_H_

