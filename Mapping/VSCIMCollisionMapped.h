
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_MAPPED_H_
#define _VSC_IM_COLLISION_MAPPED_H_

#include "VSCIMCollisionMapping.h"
#include "VSCIMTarget.h"

#include <map>

namespace VSC {
    
    namespace IM {
        
        class CollisionMapped {
            
        public:
            
            CollisionMapping::SPtr getMappingForTarget(const Target target);
            void setMappingForTarget(Target target, CollisionMapping::SPtr mapping);
            void removeMappingForTarget(const Target target);
            const Targets getMappingTargets();
            
            Target getTargetForMapping(CollisionMapping::SPtr mapping);
            
            const Targets& getExpectedMappingTargets(void) {return mExpectedTargets;}
            
            virtual void createDefaultMappings() = 0;
            
        protected:
            
            void addExpectedMappingTarget(Target target);
            
            bool checkExpectedMappingTargets(void);
            
        private:
            
            Targets mExpectedTargets;
            
            typedef std::map<Target, CollisionMapping::SPtr> CollisionMappingMap;
            
            CollisionMappingMap mCollisionMappingMap;
        };
        
    }
    
}



#endif // _VSC_IM_COLLISION_MAPPED_H_
