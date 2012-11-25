//
//  VSCIMOSXCollisionMappingListView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/8/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXCollisionEventChainController.h"

#include "VSCIMCollisionAction.h"

@class PXListView;
@protocol VSCOSXOBSceneElementController;

/*
 *  Container view for mapping views.
 */

@interface VSCIMOSXCollisionMappingListView : NSView

@property (nonatomic, assign) VSC::IM::CollisionAction::WPtr action;

@property (weak) id<VSCOSXOBSceneElementController> elementController;

@property (weak) IBOutlet PXListView* listView;

@end
