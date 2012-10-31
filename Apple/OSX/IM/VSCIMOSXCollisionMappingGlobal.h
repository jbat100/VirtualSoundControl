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

@protocol VSCIMOSXCollisionMappingEditor;

/*
 *  Handles the swapping of mapping in the model
 */

@protocol VSCIMOSXCollisionMappingController <NSObject>

-(VSC::IM::CollisionMapping::SPtr) collisionMappingEditor:(id<VSCIMOSXCollisionMappingEditor>)editor
                                  requestsMappingWithType:(VSCIMOSXCollisionMappingType)mappingType;

@end

/*
 *  Used by editor views for example
 */

@protocol VSCIMOSXCollisionMappingEditor <NSObject>

@property (weak) id<VSCIMOSXCollisionMappingController> controller;

@property (nonatomic, assign) VSC::IM::CollisionMapping::WPtr collisionMapping;

@end

