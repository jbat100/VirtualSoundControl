//
//  VSCOBSceneElementListController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCOSXEnvironmentController.h"
#import "VSCOBOSXSceneListener.h"

#include "VSC.h"
#include "VSCOBScene.h"
#include "VSCEnvironment.h"
#include "VSCEnvironmentTest.h"

@class VSCOBOSXSceneDisplayView;
@class VSCOSXOBSceneElementListView;
@class VSCOSXOBSceneDetailView;

@protocol VSCOSXOBSceneController <NSTableViewDataSource, NSTableViewDelegate, VSCOBOSXSceneListenerTarget>

@property (nonatomic, assign) VSC::OB::Scene::WPtr scene;

@property (assign) id<VSCOSXEnvironmentController> environmentController;

@property (weak) IBOutlet VSCOSXOBSceneElementListView* elementListView;
@property (weak) IBOutlet VSCOBOSXSceneDisplayView* sceneView;
@property (weak) IBOutlet VSCOSXOBSceneDetailView* sceneDetailView;

@property (nonatomic, assign) VSC::Float shootSpeed;

@end

@interface VSCOSXOBSceneController : NSObject <VSCOSXOBSceneController>

@property (nonatomic, assign) VSC::OB::OSXSceneListener::SPtr sceneListener;
@property (assign) IBOutlet id<VSCOSXEnvironmentController> environmentController;

-(void) resetSceneDisplayViews;

-(IBAction)checkBoxAction:(id)sender;
-(IBAction)shootSpeedSliderAction:(id)sender;

@end
