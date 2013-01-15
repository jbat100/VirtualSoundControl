
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_EVENT_CHAIN_H_
#define _VSC_IM_COLLISION_EVENT_CHAIN_H_

#include "VSCOB.h"
#include "VSCIMEvent.h"
#include "VSCIMEventChain.h"
#include "VSCTaskQueue.h"
#include "VSCIMCollisionAction.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        class CollisionEventChain : public EventChain {
            
        public:
            
            typedef boost::shared_ptr<CollisionEventChain>  SPtr;
            typedef boost::weak_ptr<CollisionEventChain>    WPtr;
            
            void perform(OB::Element_SPtr element, OB::Collision_SPtr collision);
            
            unsigned int getNumberOfActions(void);
            
        protected:
            
            /*
             *  Only accept delay and collision action events 
             */
            
            virtual bool checkEvent(Event::SPtr event);
            
        };
        
        typedef std::vector<CollisionEventChain::SPtr> CollisionEventChains;

    }
    
}

#endif // _VSC_IM_COLLISION_EVENT_CHAIN_H_
