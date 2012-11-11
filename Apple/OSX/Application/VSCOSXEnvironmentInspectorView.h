//
//  VSCOBOSXSceneControlView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCOSXEnvironmentController.h"
#import "DMTabBar.h"
#import "VSCOSXOBSceneElementListController.h"

#include "VSCEnvironment.h"

@class VSCOSXOBSceneElementInspectorWindowController;

/*
 *  Set up programatically, not through IB.
 */

@interface VSCOSXEnvironmentInspectorView : NSView

@property (nonatomic, assign) VSC::Environment::WPtr environment;
@property (weak) IBOutlet id<VSCOSXEnvironmentController> environmentController;

/*
 *  Tabbed interface
 */

@property (weak) IBOutlet DMTabBar* tabBar;
//@property (nonatomic, strong) IBOutlet NSTabView* tabView;

@property (weak) IBOutlet NSView* mainView;

/*
 *  Element list
 */

@property (strong) IBOutlet VSCOSXOBSceneElementListView* sceneElementListView;

-(void) showSceneElementList;

@end
