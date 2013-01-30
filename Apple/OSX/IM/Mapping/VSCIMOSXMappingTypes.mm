//
//  VSCIMOSXMappingEditor.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "VSC::IM::MappingTypes.h"

VSC::IM::MappingType VSC::IM::MappingTypeForMapping(VSC::IM::Mapping::SPtr collisionMapping)
{
    if (collisionMapping)
    {
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionConstantMapping>(collisionMapping)) return VSC::IM::MappingTypeConstant;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionVelocityMapping>(collisionMapping)) return VSC::IM::MappingTypeVelocity;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionDistanceMapping>(collisionMapping)) return VSC::IM::MappingTypeDistance;
    }
    
    return VSC::IM::MappingTypeNone;
}

VSC::IM::Mapping::SPtr VSCIMOSXCreateMappingWithType(VSC::IM::MappingType mappingType)
{
    switch (mappingType) {
            
        case VSC::IM::MappingTypeConstant:
            return VSC::IM::Mapping::SPtr(new VSC::IM::CollisionConstantMapping);
            break;
            
        case VSC::IM::MappingTypeVelocity:
            return VSC::IM::Mapping::SPtr(new VSC::IM::CollisionVelocityMapping);
            break;
            
        case VSC::IM::MappingTypeDistance:
            return VSC::IM::Mapping::SPtr(new VSC::IM::CollisionDistanceMapping);
            break;
            
        default:
            break;
    }
    
    return VSC::IM::Mapping::SPtr();
}

