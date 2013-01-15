
#ifndef _VSC_OGRE_BULLET_SCENE_H_
#define _VSC_OGRE_BULLET_SCENE_H_

#include "VSCOB.h"
#include "VSCOBCollision.h"
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
            
        class CollisionDetector : public VSC::Broadcaster
        {
            
        public:
            
            class Listener : public VSC::Listener {
                
            public:
                
                typedef boost::shared_ptr<Listener>    SPtr;
                typedef boost::weak_ptr<Listener>      WPtr;
                
                virtual void collisionProspectDetected(Collision_SPtr collision) {}
                virtual void collisionProspectUpdated(Collision_SPtr collision) {}
                virtual void collisionProspectEnded(Collision_SPtr collision) {}
                
                virtual void collisionDetected(Collision_SPtr collision) {}
                virtual void collisionUpdated(Collision_SPtr collision) {}
                virtual void collisionEnded(Collision_SPtr collision) {}
                
            };
            
            typedef boost::shared_ptr<CollisionDetector>    SPtr;
            typedef boost::weak_ptr<CollisionDetector>      WPtr;
            
            CollisionDetector(Scene_SPtr scene) : mScene(Scene_WPtr(scene)), mDistanceThreshold(0.1) {}
            
            const Collisions&   getCollisions(void) {return mCollisions;}
            Collisions getCollisionsForElementPair(Element_SPtr first, Element_SPtr second);
            
            Scene_SPtr getScene(void) {return mScene.lock();}
            
            virtual void updateCollisions();
            
        protected:
            
            void addCollision(Collision_SPtr collision);
            void removeCollision(Collision_SPtr collision);
            
            Collision_SPtr getCollisionsForPersistentManifold(btPersistentManifold* manifold);
            
            virtual bool checkListener(VSC::Listener::SPtr listener);
            
        private:
            
            Collisions      mCollisions;
            
            Scene_WPtr      mScene;
            
            Ogre::Real      mDistanceThreshold;
            
        };
        
    }
}

#endif //_VSC_OGRE_BULLET_SCENE_H_

