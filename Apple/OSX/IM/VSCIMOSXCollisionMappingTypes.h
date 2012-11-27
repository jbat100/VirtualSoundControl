//
//  VSCIMOSXCollisionMappingEditor.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCIMCollisionMapping.h"

typedef enum _VSCIMOSXCollisionMappingType {
    VSCIMOSXCollisionMappingTypeNone = 0,
    VSCIMOSXCollisionMappingTypeConstant,
    VSCIMOSXCollisionMappingTypeVelocity,
    VSCIMOSXCollisionMappingTypeDistance
} VSCIMOSXCollisionMappingType;

VSCIMOSXCollisionMappingType VSCIMOSXCollisionMappingTypeForCollisionMapping(VSC::IM::CollisionMapping::SPtr collisionMapping);

VSC::IM::CollisionMapping::SPtr VSCIMOSXCreateCollisionMappingWithType(VSCIMOSXCollisionMappingType mappingType);




