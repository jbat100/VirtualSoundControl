
#ifndef _VSC_OGRE_BULLET_SCENE_CONTROLLER_H_
#define _VSC_OGRE_BULLET_SCENE_CONTROLLER_H_

#include "VSC.h"
#include "VSCOB.h"
#include "VSCOBInterface.h"
#include "VSCOBKeyBindings.h"
#include "VSCOBMouseBindings.h"
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
         
        class SceneController : public InterfaceResponder, public KeyBound, public MouseBound
        {
        public:
            
            typedef boost::shared_ptr<SceneController>  SPtr;
            typedef boost::weak_ptr<SceneController>    WPtr;

            /**--------------------------------------------------------------
             *  Constructor/Destructor
             */
            SceneController();
            virtual ~SceneController(){}
            
            /**--------------------------------------------------------------
             *  Init for a given scene and shutdown
             */
            virtual void setupWithScene(Scene_SPtr scene);
            virtual void shutdown();

            /**--------------------------------------------------------------
             *  Dynamic actions
             */
            bool throwDynamicObjectPrimitive(Ogre::RenderWindow* renderWindow,
                                             PrimitiveType primitiveType, const Ogre::Vector3& velocity);
            
            /**--------------------------------------------------------------
             *  Input listener callback overrides
             */
            virtual bool mouseButtonPressed(Ogre::RenderWindow* renderWindow,
                                            const Ogre::Vector2& p, OIS::MouseButtonID buttonID);
            
            virtual bool mouseButtonReleased(Ogre::RenderWindow* renderWindow,
                                             const Ogre::Vector2& p, OIS::MouseButtonID buttonID);
            
            virtual bool mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& p, const Ogre::Vector2& movement);
            
            virtual bool mouseEntered(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& p);
            
            virtual bool mouseExited(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& p);
            
            virtual bool keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            
            virtual bool keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            
            virtual bool contextChanged(Ogre::RenderWindow* renderWindow);
            
            /**--------------------------------------------------------------
             *  Scene Getter
             */
            
            Scene_SPtr getScene(void) {return mScene.lock();}
            
            /**--------------------------------------------------------------
             *  Actions Setter/Getter
             */
            
            Float getShootSpeed(void) {return mShootSpeed;}
            void setShootSpeed(Float speed) {mShootSpeed = speed;}
            
            Float getShootSize(void) {return mShootSize;}
            void setShootSize(Float size) {mShootSize = size;}
            
            Float getImpulseForce(void);
            void setImplulseForce(Float force);
            
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
            
            Scene_WPtr mScene;
            
            /**
             *   Mouse picking, drag and drop
             */
            
            OgreBulletDynamics::RigidBody                               *mPickedBody;
            OgreBulletDynamics::TypedConstraint                         *mPickConstraint;
            Ogre::Vector3                                               mOldPickingPosition;
            Ogre::Real                                                  mOldPickingDistance;
            OgreBulletCollisions::CollisionClosestRayResultCallback     *mCollisionClosestRayResultCallback;
            Ogre::RaySceneQuery                                         *mRayQuery;
            
            Float mShootSpeed;
            Float mShootSize;
            Float mImpulseForce;
            
            const static bool mTraceUI = true;
            const static bool mTraceFrame = false;
            
        };
        
        typedef std::vector<SceneController::SPtr> SceneControllers;
        
        /*
         *  The controller should be owned by the interface system (e.g. NSWindowController for OS X)
         */
        
        class SceneControllerChain : public InterfaceResponderChain
        {
        public:
            
            void setupWithScene(Scene_SPtr scene);
            void shutdown();
            
            virtual void removeResponder(InterfaceResponder::SPtr responder);
            virtual void removeAllResponders(void);
            
            Scene_SPtr getScene() {return mScene.lock();}
            
        protected:
            
            virtual void insertResponder(InterfaceResponder::SPtr responder, InterfaceResponders::iterator it);
            
        private:
            
            Scene_WPtr         mScene;

        };
        
    }
}

#endif //_VSC_OGRE_BULLET_SCENE_CONTROLLER_H_

