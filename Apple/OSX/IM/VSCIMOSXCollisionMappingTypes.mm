//
//  VSCIMOSXCollisionMappingEditor.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "VSCIMOSXCollisionMappingTypes.h"

VSCIMOSXCollisionMappingType VSCIMOSXCollisionMappingTypeForCollisionMapping(VSC::IM::CollisionMapping::SPtr collisionMapping)
{
    if (collisionMapping)
    {
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionConstantMapping>(collisionMapping)) return VSCIMOSXCollisionMappingTypeConstant;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionVelocityMapping>(collisionMapping)) return VSCIMOSXCollisionMappingTypeVelocity;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionDistanceMapping>(collisionMapping)) return VSCIMOSXCollisionMappingTypeDistance;
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

