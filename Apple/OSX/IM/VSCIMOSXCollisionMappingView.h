//
//  VSCIMOSXCollisionMappingView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "PXListViewCell.h"
#import "VSCIMOSXCollisionMappingGlobal.h"

#include "VSCIMTarget.h"
#include "VSCIMCollisionMapping.h"

@interface VSCIMOSXCollisionMappingView : PXListViewCell <VSCIMOSXCollisionMappingEditor>

+(CGFloat) defaultHeightOfView;
+(CGFloat) heightOfViewForCollisionMapping:(VSC::IM::CollisionMapping::SPtr)collisionMapping;

+(NSString*) menuItemStringForCollisionMappingType:(VSCIMOSXCollisionMappingType)mappingType;
+(VSCIMOSXCollisionMappingType) collisionMappingTypeForMenuItemString:(NSString*)menuItemString;

@property (nonatomic, assign) VSC::IM::Target target;

@end

