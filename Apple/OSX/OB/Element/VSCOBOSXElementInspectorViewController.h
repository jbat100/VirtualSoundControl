//
//  VSCOBOSXElementInspectorViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCOBOSXElementController.h"
#import "VSCOBOSXSceneListener.h"

@class DMTabBar;
@class VSCOBOSXElementDetailView;
@class VSCOBOSXElementEventChainsView;
@class VSCIMOSXEventChainViewController;

@interface VSCOBOSXElementInspectorViewController : NSViewController
<VSCOBOSXElementController, VSCOBOSXSceneListenerTarget, NSTableViewDataSource, NSTableViewDelegate>

@property (strong) IBOutlet VSCOBOSXElementDetailView* elementDetailView;
@property (strong) IBOutlet VSCOBOSXElementEventChainsView* elementEventChainsView;

@property (weak) IBOutlet DMTabBar* tabBar;

-(void) setupIfNecessary;

-(void) showElementDetailView;
-(void) showElementEventChainsView;

-(IBAction) immobilized:(id)sender;

-(void) reloadImmobilizedInterface;

@end
