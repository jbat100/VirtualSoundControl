//
//  VSCOBElementListController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCOSXEnvironmentController.h"
#import "VSCOBOSXSceneListener.h"

#include "VSC.h"
#include "VSCOB.h"

@class VSCOBOSXSceneDisplayView;
@class VSCOBOSXElementListView;
@class VSCOBOSXSceneDetailView;

@protocol VSCOBOSXSceneController <NSTableViewDataSource, NSTableViewDelegate, VSCOBOSXSceneListenerTarget>

@property (nonatomic, assign) VSC::OB::Scene_WPtr scene;

@property (assign) id<VSCOSXEnvironmentController> environmentController;

@property (nonatomic, weak) IBOutlet VSCOBOSXElementListView* elementListView;
@property (nonatomic, weak) IBOutlet VSCOBOSXSceneDisplayView* sceneView;
@property (nonatomic, weak) IBOutlet VSCOBOSXSceneDetailView* sceneDetailView;

@property (nonatomic, assign) VSC::Float shootSpeed;
@property (nonatomic, assign) VSC::Float shootSize;
@property (nonatomic, assign) VSC::Float cameraSpeed;
@property (nonatomic, assign) VSC::Float cameraSensitivity;

@property (nonatomic, assign) BOOL pausePhysics;
@property (nonatomic, assign) BOOL drawWireFrame;
@property (nonatomic, assign) BOOL drawAABB;
@property (nonatomic, assign) BOOL drawContactPoints;
@property (nonatomic, assign) BOOL text;
@property (nonatomic, assign) BOOL featureText;
@property (nonatomic, assign) BOOL enableBulletLCPI;
@property (nonatomic, assign) BOOL enableCCD;
@property (nonatomic, assign) BOOL enableSatComparison;
@property (nonatomic, assign) BOOL profileTimings;

@end

@interface VSCOBOSXSceneController : NSObject <VSCOBOSXSceneController>

@property (nonatomic, assign) VSC::OB::OSXSceneListener::SPtr sceneListener;
@property (assign) IBOutlet id<VSCOSXEnvironmentController> environmentController;

@property (nonatomic, weak) IBOutlet VSCOBOSXElementListView* elementListView;
@property (nonatomic, weak) IBOutlet VSCOBOSXSceneDisplayView* sceneView;
@property (nonatomic, weak) IBOutlet VSCOBOSXSceneDetailView* sceneDetailView;

-(void) resetSceneDisplayViews;

-(IBAction)checkBoxAction:(id)sender;
-(IBAction)shootSpeedSliderAction:(id)sender;
-(IBAction)cameraSpeedSliderAction:(id)sender;
-(IBAction)cameraSensitivitySliderAction:(id)sender;

@end
