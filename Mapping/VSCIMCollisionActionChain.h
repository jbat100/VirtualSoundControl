
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_ACTION_CHAIN_H_
#define _VSC_IM_COLLISION_ACTION_CHAIN_H_

#include "VSCOBScene.h"
#include "VSCTaskQueue.h"
#include "VSCIMCollisionAction.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        class CollisionActionChain {
            
        public:
            
            typedef boost::shared_ptr<CollisionActionChain> SPtr;
            
            void perform(OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision);
            
            unsigned int numberOfActions(void);
            
            void appendAction(CollisionAction::SPtr action);
            void prependAction(CollisionAction::SPtr action);
            
            void insertActionAtIndex(CollisionAction::SPtr action, unsigned int index);
            
            void swapActions(CollisionAction::SPtr firstAction, CollisionAction::SPtr secondAction);
            
        private:
            
            CollisionActions    mActions;
            
        };
        
        typedef std::vector<CollisionActionChain::SPtr> CollisionActionChains;

    }
    
}

#endif // _VSC_IM_COLLISION_ACTION_CHAIN_H_
