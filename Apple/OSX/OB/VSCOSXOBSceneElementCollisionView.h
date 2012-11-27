//
//  VSCOSXOBSceneElementCollisionEventChainView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol VSCOSXOBSceneElementController;

#include "VSCOBScene.h"

/*
 *  A view for displaying collision related element stuff, it can show multiple
 *  collision edit views for different stages of the collision.
 */

@interface VSCOSXOBSceneElementCollisionView : NSView

@property (assign) id<VSCOSXOBSceneElementController> elementController;

@property (weak) IBOutlet NSBox* collisionStartedEventChainBox;
@property (weak) IBOutlet NSBox* collisionEndedEventChainBox;

-(void) reloadInterface;

@end
