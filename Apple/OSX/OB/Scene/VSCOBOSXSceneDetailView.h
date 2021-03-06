//
//  VSCOBOSXSceneDetailView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/17/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCOBScene.h"

@class VSCOSXKeyedCheckBoxView;
@class VSCOSXKeyedSliderView;
@protocol VSCOBOSXSceneController;

/*
 *  A view to display scene settings
 */

@interface VSCOBOSXSceneDetailView : NSView

@property (weak) IBOutlet id<VSCOBOSXSceneController> sceneController;

@property (weak) IBOutlet VSCOSXKeyedCheckBoxView *pausePhysicsCheckBoxView;
@property (weak) IBOutlet VSCOSXKeyedCheckBoxView *drawWireFrameCheckBoxView;
@property (weak) IBOutlet VSCOSXKeyedCheckBoxView *drawAABBCheckBoxView;
@property (weak) IBOutlet VSCOSXKeyedCheckBoxView *drawContactPointsCheckBoxView;
@property (weak) IBOutlet VSCOSXKeyedCheckBoxView *textCheckBoxView;
@property (weak) IBOutlet VSCOSXKeyedCheckBoxView *featureTextCheckBoxView;
@property (weak) IBOutlet VSCOSXKeyedCheckBoxView *enableBulletLCPICheckBoxView;
@property (weak) IBOutlet VSCOSXKeyedCheckBoxView *enableCCDCheckBoxView;
@property (weak) IBOutlet VSCOSXKeyedCheckBoxView *enableSatComparisonCheckBoxView;
@property (weak) IBOutlet VSCOSXKeyedCheckBoxView *profileTimingsCheckBoxView;

@property (weak) IBOutlet VSCOSXKeyedSliderView *shootSpeedSliderView;
@property (weak) IBOutlet VSCOSXKeyedSliderView *shootSizeSliderView;
@property (weak) IBOutlet VSCOSXKeyedSliderView *cameraSpeedSliderView;
@property (weak) IBOutlet VSCOSXKeyedSliderView *cameraSensitivitySliderView;

@end
