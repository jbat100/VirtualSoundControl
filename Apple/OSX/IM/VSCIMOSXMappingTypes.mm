//
//  VSCIMOSXMappingEditor.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "VSCIMOSXMappingTypes.h"

VSCIMOSXMappingType VSCIMOSXMappingTypeForMapping(VSC::IM::Mapping::SPtr collisionMapping)
{
    if (collisionMapping)
    {
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionConstantMapping>(collisionMapping)) return VSCIMOSXMappingTypeConstant;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionVelocityMapping>(collisionMapping)) return VSCIMOSXMappingTypeVelocity;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionDistanceMapping>(collisionMapping)) return VSCIMOSXMappingTypeDistance;
    }
    
    return VSCIMOSXMappingTypeNone;
}

VSC::IM::Mapping::SPtr VSCIMOSXCreateMappingWithType(VSCIMOSXMappingType mappingType)
{
    switch (mappingType) {
            
        case VSCIMOSXMappingTypeConstant:
            return VSC::IM::Mapping::SPtr(new VSC::IM::CollisionConstantMapping);
            break;
            
        case VSCIMOSXMappingTypeVelocity:
            return VSC::IM::Mapping::SPtr(new VSC::IM::CollisionVelocityMapping);
            break;
            
        case VSCIMOSXMappingTypeDistance:
            return VSC::IM::Mapping::SPtr(new VSC::IM::CollisionDistanceMapping);
            break;
            
        default:
            break;
    }
    
    return VSC::IM::Mapping::SPtr();
}

