//
//  VSCOSXOBSceneDetailView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/17/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCOBScene.h"

@protocol VSCOSXOBSceneController;

/*
 *  A view to display scene settings
 */

@interface VSCOSXOBSceneDetailView : NSView

@property (weak) IBOutlet id<VSCOSXOBSceneController> sceneController;

@property (weak) IBOutlet NSButton *pausePhysicsCheckBox;

@property (weak) IBOutlet NSButton *drawWireFrameCheckBox;
@property (weak) IBOutlet NSButton *drawAABBCheckBox;
@property (weak) IBOutlet NSButton *drawContactPointsCheckBox;

@property (weak) IBOutlet NSButton *textCheckBox;
@property (weak) IBOutlet NSButton *featureTextCheckBox;

@property (weak) IBOutlet NSButton *enableBulletLCPICheckBox;
@property (weak) IBOutlet NSButton *enableCCDCheckBox;
@property (weak) IBOutlet NSButton *enableSatComparisonCheckBox;

@property (weak) IBOutlet NSButton *profileTimingsCheckBox;

-(void) reloadInterface;
-(void) reloadSetting:(VSC::OB::Scene::Setting)setting;

@property (nonatomic, strong, readonly) NSArray* allCheckBoxes;
-(VSC::OB::Scene::Setting) settingForCheckBox:(id)checkBox;

@end
