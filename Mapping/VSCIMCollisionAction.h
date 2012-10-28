
#ifndef _VSC_IM_COLLISION_ACTION_H_
#define _VSC_IM_COLLISION_ACTION_H_

#include "VSCIMAction.h"
#include "VSCIMCollisionMapped.h"
#include "VSCOBScene.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        class CollisionAction : public Action, public CollisionMapped {
            
        public:
            
            typedef boost::shared_ptr<CollisionAction> SPtr;
            
            CollisionAction() {}
            
            virtual Task::SPtr createTaskForCollision(OB::Scene::Element::SPtr element,
                                                      OB::Scene::Collision::SPtr collision) = 0;
            
        };

        typedef std::vector<CollisionAction::SPtr> CollisionActions;

    }
    
}

#endif // _VSC_IM_COLLISION_ACTION_H_
