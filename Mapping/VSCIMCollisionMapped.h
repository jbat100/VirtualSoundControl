
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_MAPPED_H_
#define _VSC_IM_COLLISION_MAPPED_H_

#include "VSCIMCollisionMapping.h"

#include <map>

namespace VSC {
    
    namespace IM {
    
        enum Target {
            None = 0,
            Channel,
            Pitch,
            VelocityOn,
            VelocityOff,
            Duration,
            ControlValue
        };
        
        class CollisionMapped {
            
        public:
            
            CollisionMapping::SPtr getMappingForTarget(Target target);
            void setMappingForTarget(Target target, CollisionMapping::SPtr mapping);
            
        private:
            
            typedef std::map<Target, CollisionMapping::SPtr> CollisionMappingMap;
            
            CollisionMappingMap mCollisionMappingMap;
        };
        
    }
    
}



#endif // _VSC_IM_COLLISION_MAPPED_H_
