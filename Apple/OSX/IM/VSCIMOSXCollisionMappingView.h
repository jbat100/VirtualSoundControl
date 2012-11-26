//
//  VSCIMOSXCollisionMappingView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "PXListViewCell.h"
#import "VSCOSXOBSceneElementEditor.h"
#import "VSCIMOSXCollisionMappingGlobal.h"

#include "VSCIMTarget.h"
#include "VSCIMCollisionMapping.h"

@protocol VSCIMOSXCollisionEventChainEditor;

@interface VSCIMOSXCollisionMappingView : PXListViewCell

+(CGFloat) defaultHeightOfView;
+(CGFloat) heightOfViewForCollisionMapping:(VSC::IM::CollisionMapping::SPtr)collisionMapping;

+(NSString*) menuItemStringForCollisionMappingType:(VSCIMOSXCollisionMappingType)mappingType;
+(VSCIMOSXCollisionMappingType) collisionMappingTypeForMenuItemString:(NSString*)menuItemString;

@property (nonatomic, assign) id<VSCIMOSXCollisionEventChainEditor> eventChainEditor;

@property (nonatomic, assign) VSC::IM::CollisionMapping::WPtr mapping;
@property (nonatomic, assign) VSC::IM::Target target;

@end

