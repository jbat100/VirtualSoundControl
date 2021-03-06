//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_MAPPER_H_
#define _VSC_IM_COLLISION_MAPPER_H_

#include "VSCOB.h"
#include "VSCOBCollisionDetector.h"
#include "VSCIMCollisionAction.h"
#include "VSCIMCollisionEventChain.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <map>

namespace VSC {
    
    namespace IM {
        
        class CollisionMapper : public VSC::OB::CollisionDetector::Listener {
            
        public:
            
            typedef boost::shared_ptr<CollisionMapper>    SPtr;
            typedef boost::weak_ptr<CollisionMapper>      WPtr;
            
            /**
             *  Action Chains
             */
            
            void addActionChainForCollisionStarted(CollisionEventChain::SPtr actionChain, OB::Element_SPtr element);
            void addActionChainForCollisionEnded(CollisionEventChain::SPtr actionChain, OB::Element_SPtr element);
            
            void removeActionChainForCollisionStarted(CollisionEventChain::SPtr actionChain, OB::Element_SPtr element);
            void removeActionChainForCollisionEnded(CollisionEventChain::SPtr actionChain, OB::Element_SPtr element);
            
            CollisionEventChains& getEventChainsForCollisionStarted(OB::Element_SPtr element);
            CollisionEventChains& getEventChainsForCollisionEnded(OB::Element_SPtr element);
            
            /**
             *  CollisionDetector::Listener
             */
            
            virtual void collisionProspectDetected(OB::Collision_SPtr collision);
            virtual void collisionProspectUpdated(OB::Collision_SPtr collision);
            virtual void collisionProspectEnded(OB::Collision_SPtr collision);
            
            virtual void collisionDetected(OB::Collision_SPtr collision);
            virtual void collisionUpdated(OB::Collision_SPtr collision);
            virtual void collisionEnded(OB::Collision_SPtr collision);
            
        private:
            
            typedef std::map<OB::Element_SPtr, CollisionEventChains> CollisionEventChainMap;
            
            CollisionEventChainMap mCollisionStartedEventChainMap;
            CollisionEventChainMap mCollisionEndedEventChainMap;
            
            static const bool mTraceCollisions = true;
            
        };
        
    }
    
}


#endif // _VSC_IM_COLLISION_MAPPER_H_
