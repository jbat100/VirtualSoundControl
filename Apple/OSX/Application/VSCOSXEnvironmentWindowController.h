//
//  VSCOBOSXSceneWindow.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCOSXEnvironmentController.h"
#import "VSCOBOSXSceneListener.h"

#include "VSCEnvironment.h"

@class VSCOBOSXSceneDisplayView;
@class VSCOSXOBSceneElementInspectorWindowController;
@class VSCOSXOBSceneElementListView;
@class VSCOSXOBSceneDetailView;
@class VSCIMOSXEventChainListView;

/*
 *  This is the main window controller 
 */

@interface VSCOSXEnvironmentWindowController : NSWindowController <VSCOSXEnvironmentController>

@property (strong) VSCOSXOBSceneElementInspectorWindowController* elementInspectorWindowController;

@property (nonatomic, weak) IBOutlet NSSplitView* splitView;
@property (nonatomic, strong) IBOutlet NSView* environmentInspectorView;
@property (nonatomic, strong) IBOutlet NSScrollView* sceneDetailScrollView;

@property (nonatomic, strong) IBOutlet VSCOSXOBSceneElementListView* sceneElementListView;
@property (nonatomic, strong) IBOutlet VSCOSXOBSceneDetailView* sceneDetailView;
@property (nonatomic, strong) IBOutlet VSCIMOSXEventChainListView* eventChainListView;

@property (nonatomic, assign) VSC::EnvironmentTest::SPtr environmentTest;

-(IBAction)resetAction:(id)sender;

-(void) showSceneElementList;
-(void) showSceneDetail;

- (IBAction)test1:(id)sender;
- (IBAction)test2:(id)sender;
- (IBAction)test3:(id)sender;

@end
