//
//  VSCOBOSXSceneWindow.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCOSXApplicationManagerProtocol.h"
#include "VSCOBScene.h"

@class VSCOBOSXSceneView;

@interface VSCOBOSXSceneWindowController : NSWindowController

//@property (nonatomic, strong) IBOutletCollection(VSCOBOSXSceneView) NSArray* ogreBulletSceneViews;

@property (nonatomic, strong) IBOutlet VSCOBOSXSceneView* ogreBulletSceneView;

@property (nonatomic, strong) IBOutlet NSPopUpButton* scenePopUpButton;

@property (nonatomic, assign) VSC::OB::Scene::SPtr ogreBulletScene;

@property (nonatomic, weak) id<VSCOSXApplicationManagerProtocol> applicationManager;

- (IBAction)test1:(id)sender;
- (IBAction)test2:(id)sender;
- (IBAction)test3:(id)sender;

- (IBAction)showMidiWindow:(id)sender;

- (IBAction)scenePopUpButtonChanged:(id)sender;

@end
