//
//  VSCOBOSXSceneControlView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCEnvironment.h"

@class VSCOSXOBSceneElementInspectorWindowController;
@class DMTabBar;
@class VSCOSXOBSceneElementListView;
@class VSCOSXOBSceneDetailView;
@protocol VSCOSXEnvironmentController;

/*
 *  Set up programatically, not through IB.
 */

@interface VSCOSXEnvironmentInspectorView : NSView

@property (nonatomic, assign) VSC::Environment::WPtr environment;
@property (assign) IBOutlet id<VSCOSXEnvironmentController> environmentController;

@property (strong) IBOutlet VSCOSXOBSceneElementListView* sceneElementListView;
@property (strong) IBOutlet VSCOSXOBSceneDetailView* sceneDetailView;

-(void) showSceneElementList;
-(void) showSceneDetail;

@end
