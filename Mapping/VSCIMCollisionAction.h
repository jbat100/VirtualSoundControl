
#ifndef _VSC_IM_COLLISION_ACTION_H_
#define _VSC_IM_COLLISION_ACTION_H_

#include "VSCIMAction.h"
#include "VSCIMCollisionMapped.h"
#include "VSCOBScene.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        /*
         *  Abstract collision action, with a task creater taking collision and collidee.
         */
        
        class CollisionAction : public Action, public CollisionMapped {
            
        public:
            
            typedef boost::shared_ptr<CollisionAction>  SPtr;
            typedef boost::weak_ptr<CollisionAction>    WPtr;
            
            CollisionAction() : mMuted(false) {}
            
            bool isMuted(void) {return mMuted;}
            void setMuted(bool mute) {mMuted = mute;}
            
            virtual void createDefaultMappings() {}
            
            virtual Task::SPtr createTaskForCollision(OB::Scene::Element::SPtr element,
                                                      OB::Scene::Collision::SPtr collision) = 0;
            
        private:
            
            bool mMuted;
            
        };

        typedef std::vector<CollisionAction::SPtr> CollisionActions;
        
        /*
         *  Empty collision action used as default when creating a new action in GUI
         *  so that I don't have to choose another default action (which wouldn't make
         *  sense anyway)
         *
         *  I've come to the conclusion that this is not actually necessary as
         *  we can just create an empty action view.
         */
        
        class CollisionVoidAction : public Action {
            
        public:
            
            virtual void createDefaultMappings() {}
            
            virtual Task::SPtr createTaskForCollision(OB::Scene::Element::SPtr element,
                                                      OB::Scene::Collision::SPtr collision);
            
        };

    }
    
}

#endif // _VSC_IM_COLLISION_ACTION_H_
