//
//  VSCIMOSXActionMappingsController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VSCIMOSXMappingTypes.h"

#include "VSCIMTarget.h"
#include "VSCIMMapping.h"
#include "VSCIMAction.h"

@protocol VSCIMOSXActionMappingsController <NSObject>

@property (nonatomic, assign) VSC::IM::Action::WPtr action;

-(VSC::IM::Mapping::SPtr) sender:(id)sender
                  requestsMappingWithType:(VSCIMOSXMappingType)mappingType
                                forTarget:(VSC::IM::Target)target;

-(void) sender:(id)sender requestsEditorForMapping:(VSC::IM::Mapping::SPtr)mapping;

@end

