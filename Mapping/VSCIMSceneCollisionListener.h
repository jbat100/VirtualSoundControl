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

#include <boost/shared_ptr>
#include <boost/weak_ptr>

namespace VSC {
    
    namespace IM {
        
        class CollisionListener : public VSC::OB::Scene::CollisionListener {
            
        public:
            
            typedef boost::shared_ptr<CollisionListener> 
            
            virtual void collisionProspectDetected(Scene::Collision::SPtr collision);
            virtual void collisionProspectUpdated(Scene::Collision::SPtr collision);
            virtual void collisionProspectEnded(Scene::Collision::SPtr collision);
            
            virtual void collisionDetected(Scene::Collision::SPtr collision);
            virtual void collisionUpdated(Scene::Collision::SPtr collision);
            virtual void collisionEnded(Scene::Collision::SPtr collision);
            
        private:
            
            static const bool mTraceCollisions = true;
            
        };
        
    }
    
}


#endif // _VSC_IM_COLLISION_LISTENER_H_
