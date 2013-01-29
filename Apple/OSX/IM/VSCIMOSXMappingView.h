//
//  VSCIMOSXMappingView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXMappingTypes.h"

#include "VSCIMTarget.h"
#include "VSCIMMapping.h"

@protocol VSCIMOSXActionMappingsController;

@protocol VSCIMOSXMappingView <NSObject>

@property (assign) id<VSCIMOSXActionMappingsController> controller;
@property (nonatomic, assign) VSC::IM::Mapping::WPtr mapping;
@property (nonatomic, assign) VSC::IM::Target target;

@end


@interface VSCIMOSXMappingView : NSView <VSCIMOSXMappingView>

+(CGFloat) defaultHeight;
+(CGFloat) heightOfViewForMapping:(VSC::IM::Mapping::SPtr)collisionMapping;
+(NSString*) menuItemStringForMappingType:(VSCIMOSXMappingType)mappingType;
+(VSCIMOSXMappingType) collisionMappingTypeForMenuItemString:(NSString*)menuItemString;

@property (assign) id<VSCIMOSXActionMappingsController> controller;
@property (weak) IBOutlet NSButton* editButton;

-(void) reloadInterface;

@end

