//
//  VSCOSXOBSceneElementCollisionEventChainView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VSCIMOSXCollisionEventChainView;
@class VSCIMOSXCollisionEventChainController;
@class VSCIMOSXCollisionMappingListView;
@protocol VSCOSXOBSceneElementController;

#include "VSCOBScene.h"

/*
 *  A view for displaying collision related element stuff, it can show multiple
 *  collision edit views for different stages of the collision.
 */

@interface VSCOSXOBSceneElementCollisionView : NSView

@property (nonatomic, weak) id<VSCOSXOBSceneElementController> elementController;

@property (weak) IBOutlet NSView* collisionStartedContainerView;
@property (weak) IBOutlet NSView* collisionEndedContainerView;

@property (strong) VSCIMOSXCollisionEventChainView* collisionStartedEventChainView;
@property (strong) VSCIMOSXCollisionEventChainView* collisionEndedEventChainView;

-(void) reloadInterface;

@end
