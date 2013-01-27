//
//  VSCOSXOBSceneElementInspectorViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCOSXOBSceneElementController.h"
#import "VSCOBOSXSceneListener.h"

@class DMTabBar;
@class VSCOSXOBSceneElementDetailView;
@class VSCOSXOBSceneElementCollisionView;
@class VSCIMOSXCollisionEventChainViewController;

@interface VSCOSXOBSceneElementInspectorViewController : NSViewController
<VSCOSXOBSceneElementController, VSCOBOSXSceneListenerTarget>

@property (strong) IBOutlet VSCOSXOBSceneElementDetailView* elementDetailView;
@property (strong) IBOutlet VSCOSXOBSceneElementCollisionView* elementCollisionView;

@property (strong) VSCIMOSXCollisionEventChainViewController* collisionStartedEventChainViewController;
@property (strong) VSCIMOSXCollisionEventChainViewController* collisionEndedEventChainViewController;

@property (weak) IBOutlet DMTabBar* tabBar;

-(void) showElementDetailView;
-(void) showElementCollisionView;

-(IBAction) immobilized:(id)sender;

-(void) reloadImmobilizedInterface;

@end
