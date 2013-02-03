//
//  VSCIMOSXEventEditor.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VSC::IM::MappingTypes.h"

#include "VSCIM.h"
#include "VSCIMTarget.h"


@protocol VSCIMOSXEventEditor <NSObject>

@property (nonatomic, assign) VSC::IM::Action::WPtr action;

-(VSC::IM::Mapping_SPtr) sender:(id)sender
requestsMappingWithType:(VSC::IM::MappingType)mappingType
                       forTarget:(VSC::IM::Target)target;

-(void) sender:(id)sender requestsEditorForMapping:(VSC::IM::Mapping_SPtr)mapping;

@end

