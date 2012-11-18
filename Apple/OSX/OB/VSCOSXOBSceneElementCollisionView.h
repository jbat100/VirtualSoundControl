//
//  VSCOSXOBSceneElementCollisionEventChainView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VSCIMOSXCollisionEventChainView;
@protocol VSCOSXOBSceneElementController;

/*
 *  A view for displaying collision related element stuff
 */

@interface VSCOSXOBSceneElementCollisionView : NSView

@property (weak) id<VSCOSXOBSceneElementController> elementController;

@property (strong) VSCIMOSXCollisionEventChainView* eventChainView;

-(void) reloadInterface;

@end
