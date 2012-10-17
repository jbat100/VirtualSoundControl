
#ifndef _VSC_OGRE_BULLET_SCENE_CONTROLLER_H_
#define _VSC_OGRE_BULLET_SCENE_CONTROLLER_H_

#include "VSCOB.h"
#include "VSCOBInputListener.h"
#include "VSCOBKeyBindings.h"
#include "VSCOBScene.h"

#include "OgreBulletDynamics.h"
#include "OIS.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace VSC {
    
    namespace OB {

        /**
         *  Used to translate user input to scene actions.
         */
         
        class SceneController : public InputListener, public KeyBound
        {
        public:
            
            typedef boost::shared_ptr<SceneController>  SPtr;
            typedef boost::weak_ptr<SceneController>    WPtr;

            /**--------------------------------------------------------------
             *  Constructor/Destructor
             */
            SceneController();
            SceneController(Scene::WPtr scene);
            virtual ~SceneController(){}
            
            /**--------------------------------------------------------------
             *  Init for a given scene and shutdown
             */
            virtual void setupWithScene(Scene::WPtr scene);
            virtual void shutdown();

            /**--------------------------------------------------------------
             *  Ogre Frame Listener Forwarded messages from Application 
             */
            //virtual bool frameStarted(Ogre::Real elapsedTime);
            //virtual bool frameEnded(Ogre::Real elapsedTime);

            /**--------------------------------------------------------------
             *  Dynamic actions
             */
            void throwDynamicObjectPrimitive(PrimitiveType primitiveType, const Ogre::Vector3& velocity);
            
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
             *  Scene Getter
             */
            
            Scene::WPtr getScene(void) {return mScene;}
            void setScene(Scene::WPtr scene) {mScene = scene;}
            
            /**--------------------------------------------------------------
             *  Actions Setter/Getter
             */
            
            float getShootSpeed(void);
            void setShootSpeed(float speed);
            
            float getImpulseForce(void);
            void setImplulseForce(float force);
            
            /**--------------------------------------------------------------
             *  Utilities
             */
            Ogre::Vector2 normalizedViewportCoordinates(const Ogre::Vector2& absoluteCoordinates);
            
        protected:
            
            /**--------------------------------------------------------------
             *   Mouse picking Setters/Getters
             */
            
            OgreBulletDynamics::RigidBody* getPickedBody(void);
            void setPickedBody(OgreBulletDynamics::RigidBody* body);
            
            OgreBulletDynamics::TypedConstraint* getTypedConstraint(void);
            void setTypedConstraint(OgreBulletDynamics::TypedConstraint* constraint);
            
            Ogre::Vector3 getOldPickingPosition(void);
            void setOldPickingPosition(Ogre::Vector3 position);
            
            Ogre::Real getOldPickingDistance(void);
            void setOldPickingDistance(Ogre::Real distance);
            
            OgreBulletCollisions::CollisionClosestRayResultCallback* getCollisionClosestRayResultCallback(void);
            void setCollisionClosestRayResultCallback(OgreBulletCollisions::CollisionClosestRayResultCallback* callback);
            
        private:
            
            Scene::WPtr mScene;
            
            /**
             *   Mouse picking, drag and drop
             */
            
            OgreBulletDynamics::RigidBody                               *mPickedBody;
            OgreBulletDynamics::TypedConstraint                         *mPickConstraint;
            Ogre::Vector3                                               mOldPickingPosition;
            Ogre::Real                                                  mOldPickingDistance;
            OgreBulletCollisions::CollisionClosestRayResultCallback     *mCollisionClosestRayResultCallback;
            Ogre::RaySceneQuery                                         *mRayQuery;
            
            float mShootSpeed;
            float mImpulseForce;
            
            const static bool mTraceUI = false;
            const static bool mTraceFrame = false;
            
        };
        
        typedef std::vector<SceneController::SPtr> SceneControllers;
        
        /*
         *  The controller should be owned by the interface system (e.g. NSWindowController for OS X)
         */
        
        class SceneControllerChain : public InputListener
        {
        public:
            
            void setupWithScene(Scene::SPtr scene);
            void shutdown();

            const SceneControllers& getSceneControllers(void) {return mSceneControllers;}
            
            void appendSceneController(SceneController::SPtr controller);
            void prependSceneController(SceneController::SPtr controller);
            void insertSceneControllerAfterSceneController(SceneController::SPtr controller);
            void insertSceneControllerBeforeSceneController(SceneController::SPtr controller);
            
            void removeSceneController(SceneController::SPtr controller);
            void removeAllSceneControllers(void);
            
        protected:
            
            void chainSceneControllers(void);
            
            void insertSceneController(SceneController::SPtr controller, SceneControllers::iterator it);
            
        private:
            
            Scene::WPtr         mScene;
            
            SceneControllers    mSceneControllers;
        };
        
    }
}

#endif //_VSC_OGRE_BULLET_SCENE_CONTROLLER_H_
