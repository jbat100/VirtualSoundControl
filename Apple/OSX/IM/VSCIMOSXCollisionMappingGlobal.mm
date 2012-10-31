//
//  VSCIMOSXCollisionMappingEditor.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "VSCIMOSXCollisionMappingGlobal.h"

VSCIMOSXCollisionMappingType VSCIMOSXCollisionMappingTypeForCollisionMapping(VSC::IM::CollisionMapping::SPtr collisionMapping)
{
    if (collisionMapping)
    {
        VSC::IM::CollisionConstantMapping::SPtr constant = boost::dynamic_pointer_cast<VSC::IM::CollisionConstantMapping>(collisionMapping);
        if (constant) return VSCIMOSXCollisionMappingTypeConstant;
        
        VSC::IM::CollisionVelocityMapping::SPtr velocity = boost::dynamic_pointer_cast<VSC::IM::CollisionVelocityMapping>(collisionMapping);
        if (velocity) return VSCIMOSXCollisionMappingTypeVelocity;
        
        VSC::IM::CollisionDistanceMapping::SPtr distance = boost::dynamic_pointer_cast<VSC::IM::CollisionDistanceMapping>(collisionMapping);
        if (distance) return VSCIMOSXCollisionMappingTypeDistance;
    }
    
    return VSCIMOSXCollisionMappingTypeNone;
}

VSC::IM::CollisionMapping::SPtr VSCIMOSXCreateCollisionMappingWithType(VSCIMOSXCollisionMappingType mappingType)
{
    switch (mappingType) {
            
        case VSCIMOSXCollisionMappingTypeConstant:
            return VSC::IM::CollisionMapping::SPtr(new VSC::IM::CollisionConstantMapping);
            break;
            
        case VSCIMOSXCollisionMappingTypeVelocity:
            return VSC::IM::CollisionMapping::SPtr(new VSC::IM::CollisionVelocityMapping);
            break;
            
        case VSCIMOSXCollisionMappingTypeDistance:
            return VSC::IM::CollisionMapping::SPtr(new VSC::IM::CollisionDistanceMapping);
            break;
            
        default:
            break;
    }
    
    return VSC::IM::CollisionMapping::SPtr();
}

