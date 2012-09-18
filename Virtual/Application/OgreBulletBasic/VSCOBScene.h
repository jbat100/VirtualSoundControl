
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


#define BASIC_HELP_INFO0 "Use F1, F2, F3, F4"
#define BASIC_HELP_INFO1 "B,N,G,H to Throw a Cube, Sphere, Cone, Cylinder"
#define BASIC_HELP_INFO2 "J,K,I,U to Drop  a Cube, Sphere, Cone, Cylinder"
#define BASIC_HELP_INFO3 ""
#define BASIC_HELP_INFO4 "left click to drag"
#define BASIC_HELP_INFO5 "right to camera move"
#define BASIC_HELP_INFO6 "middle for impulse"

namespace VSC {
    
    namespace OB {

        enum QueryFlags
        {
            ANY_QUERY_MASK					= 1<<0,
            RAGDOLL_QUERY_MASK				= 1<<1,
            GEOMETRY_QUERY_MASK				= 1<<2,
            VEHICLE_QUERY_MASK				= 1<<3,
            STATIC_GEOMETRY_QUERY_MASK		= 1<<4
        };

        class Application;

        /*
        The base Test class, is also able to listen for collisions and thus change the contact properties
        */
        class Scene : public InputListener, public KeyBound
        {
        public:

            /**--------------------------------------------------------------
             *  Constructor/Destructor/Initialization 
             */
            Scene();
            virtual ~Scene(){};
            virtual void init(Ogre::Root *root, Ogre::RenderWindow *win, Application *application);

            /**--------------------------------------------------------------
             *  Setup/Teardown 
             */
            virtual void shutdown();
            void setPhysicGUI();
            void setBasicLight();

            /**--------------------------------------------------------------
             *  MISC Info
             */
            const Ogre::String getName() {return mName;}
            void setInfoText();
            void setDebugText(const Ogre::String &debugText){mDebugText = debugText;}

            /**--------------------------------------------------------------
             *  Ogre Frame Listener Forwarded messages from Application 
             */
            virtual bool frameStarted(Ogre::Real elapsedTime);
            virtual bool frameEnded(Ogre::Real elapsedTime);

            /**--------------------------------------------------------------
             *  Dynamic actions and checks
             */
            bool checkIfEnoughPlaceToAddObject(float maxDist);
            bool throwDynamicObject(KeyboardAction::Key key);  // returns true if something happened
            bool dropDynamicObject(KeyboardAction::Key key);   // returns true if something happened
            
            /**--------------------------------------------------------------
             *  Input listener callback overrides
             */
            virtual bool mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
            virtual bool mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
            virtual bool mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement);
            virtual bool mouseEntered(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position);
            virtual bool mouseExited(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position);
            virtual bool keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            virtual bool keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            virtual bool renderWindowChangedSize(Ogre::RenderWindow* renderWindow);
            
            /**--------------------------------------------------------------
             *  Ogre Setters/Getters
             */
            
            Ogre::RenderWindow* getRenderWindow(void) {return mWindow;};
            Ogre::Root* getRoot(void) {return mRoot;}
            Ogre::SceneManager* getSceneManager(void) {return mSceneMgr;}
            Ogre::Camera* getCamera(void) {return mCamera;}
            
        protected:

            /**
             *  World initialization (non-virtual?)
             */
            void initWorld (const Ogre::Vector3 &gravityVector = Ogre::Vector3 (0,-9.81,0), 
                            const Ogre::AxisAlignedBox &bounds = Ogre::AxisAlignedBox (Ogre::Vector3 (-10000, -10000, -10000),
                                                                                       Ogre::Vector3 (10000,  10000,  10000)));
            void addGround();

            /**
             *  Add objects to scene, no direct 
             *  QUESTION: (no mesh data for addStaticTrimesh?) why cast shadow only for static tri mesh
             */
            OgreBulletDynamics::RigidBody *addCube(const Ogre::String instanceName,
                const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Vector3 &size,
                const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
                const Ogre::Real bodyMass);
            OgreBulletDynamics::RigidBody *addCylinder(const Ogre::String instanceName,
                const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Vector3 &size,
                const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
                const Ogre::Real bodyMass);
            OgreBulletDynamics::RigidBody *addSphere(const Ogre::String instanceName,
                const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Real radius,
                const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
                const Ogre::Real bodyMass);
            OgreBulletDynamics::RigidBody *addCone(const Ogre::String instanceName,
                const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Vector3 &size,
                const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
                const Ogre::Real bodyMass);
            OgreBulletDynamics::RigidBody *addStaticTrimesh(const Ogre::String &instanceName,
                const Ogre::String &meshName, const Ogre::Vector3 &pos, const Ogre::Quaternion &q, 
                const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, const bool castShadow = true);
            OgreBulletDynamics::RigidBody *addStaticPlane( const Ogre::Real bodyRestitution, 
                                                           const Ogre::Real bodyFriction);
            
            
            
            /*
             *  Debug stuff
             */
            void getDebugLines();
            
            const static bool mTraceUI = true;

            /**
             *  Mouse picking utils 
             */
            OgreBulletDynamics::RigidBody* getBodyUnderCursorUsingBullet(Ogre::Vector3 &intersectionPoint, Ogre::Ray &rayTo);
            OgreBulletDynamics::RigidBody* getBodyUnderCursorUsingOgre(Ogre::Vector3 &intersectionPoint, Ogre::Ray &rayTo);

            
            static size_t mNumEntitiesInstanced;
            
            Ogre::RenderWindow                  *mWindow;
            Ogre::Root                          *mRoot;
            Ogre::SceneManager                  *mSceneMgr;
            Ogre::Camera                        *mCamera;
            
            int                                 mCurrentShadowTechnique;
            Ogre::Light                         *mSunLight;
            Ogre::Light                         *mLight;
            Ogre::Light                         *mLight2;

            OgreBulletDynamics::DynamicsWorld   *mWorld;
            Application            *mApplication;


            std::deque<Ogre::Entity *>                          mEntities;
            std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
            std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;

            bool  mStatsOn;
            bool  mQuit;
            bool  mDoOnestep;

            float mShootSpeed;
            float mImpulseForce;
            bool  mPaused;

            bool  mWireFrame;
            bool  mDrawAabb;
            bool  mDrawFeaturesText;
            bool  mDrawContactPoints;
            bool  mNoDeactivation;
            bool  mNoHelpText;
            bool  mDrawText;
            bool  mProfileTimings;
            bool  mEnableSatComparison;
            bool  mDisableBulletLCP;
            bool  mEnableCCD;


           BetaGUI::StaticText*     mFpsStaticText;


           /**
            *   Mouse picking, drag and drop
            */
            
           OgreBulletDynamics::RigidBody                            *mPickedBody;
           OgreBulletDynamics::TypedConstraint                      *mPickConstraint;
           Ogre::Vector3                                            mOldPickingPos;
           Ogre::Real                                               mOldPickingDist;
           OgreBulletCollisions::CollisionClosestRayResultCallback  *mCollisionClosestRayResultCallback;

           OgreBulletCollisions::DebugLines                         *mDebugRayLine;
           Ogre::RaySceneQuery                                      *mRayQuery;

           BetaGUIListener                                   *mGuiListener;
            
           Ogre::String                                             mDebugText;
           Ogre::String                                             mName;
           std::vector<Ogre::String>                                mHelpKeys;
            
           void updateStats();
            
            /*
             *  Other Setters/Getters
             */
            
            CameraController::SPtr   getCameraController(void) {return mCameraController;}
            void                            setCameraController(CameraController::SPtr controller);
            
        private:
            
            CameraController::SPtr   mCameraController;
            static const bool               mTraceFrame = false;
            
        };
        
    }
}

#endif //_VSC_OGRE_BULLET_SCENE_H_

