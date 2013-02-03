//
//  VSCIMOSXMappingView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSC::IM::MappingTypes.h"

#include "VSCIMTarget.h"
#include "VSCIMMapping.h"

@protocol VSCIMOSXEventMappingsController;

@protocol VSCIMOSXMappingView <NSObject>

@property (assign) id<VSCIMOSXEventMappingsController> controller;
@property (nonatomic, assign) VSC::IM::Mapping::WPtr mapping;
@property (nonatomic, assign) VSC::IM::Target target;

@end


@interface VSCIMOSXMappingView : NSView <VSCIMOSXMappingView>

+(CGFloat) defaultHeight;
+(CGFloat) heightOfViewForMapping:(VSC::IM::Mapping::SPtr)collisionMapping;
+(NSString*) menuItemStringForMappingType:(VSC::IM::MappingType)mappingType;
+(VSC::IM::MappingType) collisionMappingTypeForMenuItemString:(NSString*)menuItemString;

@property (assign) id<VSCIMOSXEventMappingsController> controller;
@property (weak) IBOutlet NSButton* editButton;

-(void) reloadInterface;

@end

