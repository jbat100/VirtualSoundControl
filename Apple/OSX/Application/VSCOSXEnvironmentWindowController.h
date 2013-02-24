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

@class VSCOBOSXElementInspectorWindowController;
@class VSCIMOSXEventChainWindowController;

@class VSCOBOSXSceneDisplayView;
@class VSCOBOSXElementListView;
@class VSCOBOSXSceneDetailView;
@class VSCIMOSXEventChainListView;

/*
 *  This is the main window controller 
 */

@interface VSCOSXEnvironmentWindowController : NSWindowController
<VSCOSXEnvironmentController,NSTableViewDelegate, NSTableViewDataSource>

@property (strong) IBOutlet id<VSCOBOSXSceneController> sceneController;

@property (nonatomic, strong) VSCOBOSXElementInspectorWindowController* elementInspectorWindowController;
@property (nonatomic, strong) VSCIMOSXEventChainWindowController* eventChainWindowController;

@property (nonatomic, weak) IBOutlet NSSplitView* splitView;
@property (nonatomic, strong) IBOutlet NSView* environmentInspectorView;
@property (nonatomic, strong) IBOutlet NSScrollView* sceneDetailScrollView;

@property (nonatomic, strong) IBOutlet VSCOBOSXElementListView* elementListView;
@property (nonatomic, strong) IBOutlet VSCOBOSXSceneDetailView* sceneDetailView;
@property (nonatomic, strong) IBOutlet VSCIMOSXEventChainListView* eventChainListView;

@property (nonatomic, assign) VSC::EnvironmentTest::SPtr environmentTest;

-(IBAction)resetAction:(id)sender;

-(void) showElementList;
-(void) showEventChainList;
-(void) showSceneDetail;

-(IBAction)addEventChain:(id)sender;
-(IBAction)removeSelectedEventChain:(id)sender;

-(IBAction)test1:(id)sender;
-(IBAction)test2:(id)sender;
-(IBAction)test3:(id)sender;

@end
