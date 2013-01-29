//
//  VSCIMOSXMappingEditor.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCIMMapping.h"

typedef enum _VSC::IM::MappingType {
    VSC::IM::MappingTypeNone = 0,
    VSC::IM::MappingTypeConstant,
    VSC::IM::MappingTypeVelocity,
    VSC::IM::MappingTypeDistance
} VSC::IM::MappingType;

VSC::IM::MappingType VSC::IM::MappingTypeForMapping(VSC::IM::Mapping::SPtr collisionMapping);

VSC::IM::Mapping::SPtr VSCIMOSXCreateMappingWithType(VSC::IM::MappingType mappingType);




