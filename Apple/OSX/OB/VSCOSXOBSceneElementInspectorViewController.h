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
@class VSCIMOSXEventChainViewController;

@interface VSCOSXOBSceneElementInspectorViewController : NSViewController
<VSCOSXOBSceneElementController, VSCOBOSXSceneListenerTarget>

@property (strong) IBOutlet VSCOSXOBSceneElementDetailView* elementDetailView;
@property (strong) IBOutlet VSCOSXOBSceneElementCollisionView* elementCollisionView;

@property (strong) VSCIMOSXEventChainViewController* collisionStartedEventChainViewController;
@property (strong) VSCIMOSXEventChainViewController* collisionEndedEventChainViewController;

@property (weak) IBOutlet DMTabBar* tabBar;

-(void) showElementDetailView;
-(void) showElementCollisionView;

-(IBAction) immobilized:(id)sender;

-(void) reloadImmobilizedInterface;

@end
