//
//  VSCIMOSXCollisionActionMappingsController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VSCIMOSXCollisionMappingTypes.h"

#include "VSCIMTarget.h"
#include "VSCIMCollisionMapping.h"

@protocol VSCIMOSXCollisionActionMappingsController <NSObject>

-(VSC::IM::CollisionMapping::SPtr) sender:(id)sender
                  requestsMappingWithType:(VSCIMOSXCollisionMappingType)mappingType
                                forTarget:(VSC::IM::Target);

-(void) sender:(id)sender requestsEditorForMapping:(VSC::IM::CollisionMapping::SPtr)mapping;

@end

