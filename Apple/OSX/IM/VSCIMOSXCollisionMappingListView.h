//
//  VSCIMOSXCollisionMappingListView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/8/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIMCollisionAction.h"

@class PXListView;

/*
 *  Container view for mapping views.
 */

@interface VSCIMOSXCollisionMappingListView : NSView

@property (nonatomic, assign) VSC::IM::CollisionAction::WPtr collisionAction;

@property (weak) IBOutlet PXListView* mappingListView;

@end
