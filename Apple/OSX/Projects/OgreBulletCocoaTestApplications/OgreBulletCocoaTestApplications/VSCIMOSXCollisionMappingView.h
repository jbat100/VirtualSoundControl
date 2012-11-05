//
//  VSCIMOSXCollisionMappingView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXCollisionMappingGlobal.h"

#include "VSCIMTarget.h"
#include "VSCIMCollisionMapping.h"

@interface VSCIMOSXCollisionMappingView : NSView

+(CGFloat) heightOfViewForCollisionMapping:(VSC::IM::CollisionMapping::SPtr)collisionMapping;

@property (nonatomic, assign) VSC::IM::Target target;

@property (nonatomic, assign) VSC::IM::CollisionMapping::WPtr collisionMapping;


@end

