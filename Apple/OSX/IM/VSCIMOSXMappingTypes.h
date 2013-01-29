//
//  VSCIMOSXMappingEditor.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCIMMapping.h"

typedef enum _VSCIMOSXMappingType {
    VSCIMOSXMappingTypeNone = 0,
    VSCIMOSXMappingTypeConstant,
    VSCIMOSXMappingTypeVelocity,
    VSCIMOSXMappingTypeDistance
} VSCIMOSXMappingType;

VSCIMOSXMappingType VSCIMOSXMappingTypeForMapping(VSC::IM::Mapping::SPtr collisionMapping);

VSC::IM::Mapping::SPtr VSCIMOSXCreateMappingWithType(VSCIMOSXMappingType mappingType);




