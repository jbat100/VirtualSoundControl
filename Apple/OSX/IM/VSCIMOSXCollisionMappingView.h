//
//  VSCIMOSXCollisionMappingView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXCollisionMappingTypes.h"

#include "VSCIMTarget.h"
#include "VSCIMCollisionMapping.h"

@protocol VSCIMOSXCollisionActionMappingsController;

@protocol VSCIMOSXCollisionMappingView <NSObject>

@property (assign) id<VSCIMOSXCollisionActionMappingsController> controller;
@property (nonatomic, assign) VSC::IM::CollisionMapping::WPtr mapping;
@property (nonatomic, assign) VSC::IM::Target target;

@end


@interface VSCIMOSXCollisionMappingView : NSView <VSCIMOSXCollisionMappingView>

+(CGFloat) defaultHeight;
+(CGFloat) heightOfViewForCollisionMapping:(VSC::IM::CollisionMapping::SPtr)collisionMapping;
+(NSString*) menuItemStringForCollisionMappingType:(VSCIMOSXCollisionMappingType)mappingType;
+(VSCIMOSXCollisionMappingType) collisionMappingTypeForMenuItemString:(NSString*)menuItemString;

@property (assign) id<VSCIMOSXCollisionActionMappingsController> controller;
@property (weak) IBOutlet NSButton* editButton;

-(void) reloadInterface;

@end

