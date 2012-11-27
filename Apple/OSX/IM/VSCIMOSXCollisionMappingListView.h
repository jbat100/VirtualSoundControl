//
//  VSCIMOSXCollisionMappingListView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/8/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXCollisionMappingGlobal.h"

#include "VSCIMCollisionAction.h"

@class PXListView;
@class VSCIMOSXCollisionMappingEditViewController;
@protocol VSCIMOSXCollisionEventChainController;

/*
 *  Container view for mapping views.
 */

@interface VSCIMOSXCollisionMappingListView : NSView <VSCIMOSXCollisionMappingController>

@property (weak) id<VSCIMOSXCollisionEventChainController> eventChainController;

@property (nonatomic, assign) VSC::IM::CollisionAction::WPtr action;

@property (weak) IBOutlet PXListView* listView;


@end
