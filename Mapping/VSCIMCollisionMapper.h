//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_MAPPER_H_
#define _VSC_IM_COLLISION_MAPPER_H_

#include "VSCOBScene.h"
#include "VSCIMCollisionAction.h"
#include "VSCIMCollisionEventChain.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <map>

namespace VSC {
    
    namespace IM {
        
        class CollisionMapper : public VSC::OB::Scene::CollisionDetector::Listener {
            
        public:
            
            typedef boost::shared_ptr<CollisionMapper>    SPtr;
            typedef boost::weak_ptr<CollisionMapper>      WPtr;
            
            /**
             *  Action Chains
             */
            
            void setActionChainForCollisionStarted(CollisionEventChain::SPtr actionChain, OB::Scene::Element::SPtr element);
            void setActionChainForCollisionEnded(CollisionEventChain::SPtr actionChain, OB::Scene::Element::SPtr element);
            
            CollisionEventChain::SPtr getActionChainForCollisionStarted(OB::Scene::Element::SPtr element);
            CollisionEventChain::SPtr getActionChainForCollisionEnded(OB::Scene::Element::SPtr element);
            
            /**
             *  Scene::CollisionDetector::Listener
             */
            
            virtual void collisionProspectDetected(OB::Scene::Collision::SPtr collision);
            virtual void collisionProspectUpdated(OB::Scene::Collision::SPtr collision);
            virtual void collisionProspectEnded(OB::Scene::Collision::SPtr collision);
            
            virtual void collisionDetected(OB::Scene::Collision::SPtr collision);
            virtual void collisionUpdated(OB::Scene::Collision::SPtr collision);
            virtual void collisionEnded(OB::Scene::Collision::SPtr collision);
            
        private:
            
            typedef std::map<OB::Scene::Element::SPtr, CollisionEventChain::SPtr> CollisionEventChainMap;
            
            CollisionEventChainMap mCollisionStartedEventChainMap;
            CollisionEventChainMap mCollisionEndedEventChainMap;
            
            static const bool mTraceCollisions = true;
            
        };
        
    }
    
}


#endif // _VSC_IM_COLLISION_MAPPER_H_
