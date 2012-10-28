//
//  VSCOBOSXSceneWindow.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCOSXEnvironmentController.h"

#include "VSCEnvironment.h"

@class VSCOBOSXSceneDisplayView;

@interface VSCOSXEnvironmentWindowController : NSWindowController <VSCOSXEnvironmentController>

@property (nonatomic, strong) IBOutlet VSCOBOSXSceneDisplayView* sceneView;

@property (nonatomic, assign) VSC::Environment::WPtr environment;

-(void) showElementInspectorForElement:(VSC::OB::Scene::Element::SPtr)element;

-(void) reloadInterface;

- (IBAction)test1:(id)sender;
- (IBAction)test2:(id)sender;
- (IBAction)test3:(id)sender;

@end
