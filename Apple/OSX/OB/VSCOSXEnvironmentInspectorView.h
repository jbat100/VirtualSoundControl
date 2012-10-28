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

/*
 *  Set up programatically, not through IB.
 */

@interface VSCOSXEnvironmentInspectorView : NSView

@property (nonatomic, assign) VSC::Environment::WPtr environment;
@property (weak) id<VSCOSXEnvironmentController> environmentController;

-(void) setup;

/*
 *  Tabbed interface
 */

@property (nonatomic, strong) IBOutlet DMTabBar* tabBar;
@property (nonatomic, strong) IBOutlet NSTabView* tabView;

/*
 *  Element list
 */

@property (nonatomic, strong) IBOutlet PXListView* sceneElementListView;
@property (nonatomic, strong) IBOutlet VSCOSXOBSceneElementListController* sceneElementListController;


@end
