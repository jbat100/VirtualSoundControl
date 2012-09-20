
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


namespace VSC {
    
    namespace OB {

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
            
            /*
             *  VSC::OB variables
             */
            
            Application                         *mApplication;
            
            /*
             *  Ogre variables
             */
            
            Ogre::RenderWindow                  *mWindow;
            Ogre::Root                          *mRoot;
            Ogre::SceneManager                  *mSceneMgr;
            Ogre::Camera                        *mCamera;
            Ogre::Light                         *mSunLight;
            Ogre::Light                         *mLight;
            Ogre::Light                         *mLight2;
            
            int                                 mCurrentShadowTechnique;
            

            /*
             *  OgreBullet variables
             */
            
            OgreBulletDynamics::DynamicsWorld                   *mWorld;
            
            /*
            std::deque<OgreBulletDynamics::RigidBody*>          mBodies;
            std::deque<OgreBulletCollisions::CollisionShape*>   mShapes;
            std::deque<Ogre::Entity*>           mEntities;
             */

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

           BetaGUIListener                                          *mGuiListener;
            
           Ogre::String                                             mDebugText;
           Ogre::String                                             mName;
           std::vector<Ogre::String>                                mHelpKeys;
            
           void updateStats();
            
            /*
             *  Other Setters/Getters
             */
            
            CameraController::SPtr  getCameraController(void) {return mCameraController;}
            void                    setCameraController(CameraController::SPtr controller);
            
        private:
            
            CameraController::SPtr  mCameraController;
            static const bool       mTraceFrame = false;
            
        };
        
    }
}

#endif //_VSC_OGRE_BULLET_SCENE_H_

