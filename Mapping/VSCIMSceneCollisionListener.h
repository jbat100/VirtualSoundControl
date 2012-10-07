//
//  VSCCollissionReceiver.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_LISTENER_H_
#define _VSC_IM_COLLISION_LISTENER_H_

#include "VSCOBScene.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace VSC {
    
    namespace IM {
        
        class CollisionListener : public VSC::OB::Scene::CollisionListener {
            
        public:
            
            typedef boost::shared_ptr<CollisionListener>    SPtr;
            typedef boost::weak_ptr<CollisionListener>      WPtr;
            
            virtual void collisionProspectDetected(OB::Scene::Collision::SPtr collision);
            virtual void collisionProspectUpdated(OB::Scene::Collision::SPtr collision);
            virtual void collisionProspectEnded(OB::Scene::Collision::SPtr collision);
            
            virtual void collisionDetected(OB::Scene::Collision::SPtr collision);
            virtual void collisionUpdated(OB::Scene::Collision::SPtr collision);
            virtual void collisionEnded(OB::Scene::Collision::SPtr collision);
            
        private:
            
            static const bool mTraceCollisions = true;
            
        };
        
    }
    
}


#endif // _VSC_IM_COLLISION_LISTENER_H_
