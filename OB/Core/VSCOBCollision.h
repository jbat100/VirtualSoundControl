
#ifndef _VSC_OB_COLLISION_H_
#define _VSC_OB_COLLISION_H_

#include "VSCOB.h"
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
         *  Collision objects are used for tracking and informing Collision listeners
         */
        
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
            
            Scene_SPtr getScene(void) {return mScene.lock();}
            
            Element_SPtr getFirstElement(void) const {return mFirstElement.lock();}
            Element_SPtr getSecondElement(void) const {return mSecondElement.lock();}
            
            State getState(void) {return mState;}
            
            //const PersistentManifolds& getPersistentManifolds(void) {return mPersistentManifolds;}
            
            btPersistentManifold* getPersistentManifold(void) {return mPersistentManifold;}
            
            const Ogre::Vector3& getFirstElementCollisionVelocity(void) const {return mFirstElementCollisionVelocity;}
            const Ogre::Vector3& getSecondElementCollisionVelocity(void) const {return mSecondElementCollisionVelocity;}
            const Ogre::Vector3& getRelativeCollisionVelocity(void) const {return mRelativeCollisionVelocity;}
            
        protected:
            
            void setScene(Scene_WPtr scene);
            
            void setFirstElement(Element_SPtr element) {mFirstElement = Element_WPtr(element);}
            void setSecondElement(Element_SPtr element) {mSecondElement = Element_WPtr(element);}
            
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
            
            void setFirstElementCollisionVelocity(const Ogre::Vector3& vel) {mFirstElementCollisionVelocity = vel;}
            void setSecondElementCollisionVelocity(const Ogre::Vector3& vel) {mSecondElementCollisionVelocity = vel;}
            void setRelativeCollisionVelocity(const Ogre::Vector3& vel) {mRelativeCollisionVelocity = vel;}
            
        private:
            
            Scene_WPtr              mScene;
            
            Element_WPtr            mFirstElement;
            Element_WPtr            mSecondElement;
            
            State                   mState;
            
            btPersistentManifold*   mPersistentManifold;
            
            Ogre::Vector3           mFirstElementCollisionVelocity;
            Ogre::Vector3           mSecondElementCollisionVelocity;
            Ogre::Vector3           mRelativeCollisionVelocity;
            
        };
        
        std::ostream& operator << (std::ostream& stream, const Collision& collision);
        
    }
}

#endif //_VSC_OB_COLLISION_H_

