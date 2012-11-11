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
@class VSCOSXOBSceneElementInspectorWindowController;
@class VSCOSXEnvironmentInspectorView;

/*
 *  This is the main window controller 
 */

@interface VSCOSXEnvironmentWindowController : NSWindowController <VSCOSXEnvironmentController>

@property (strong) IBOutlet VSCOBOSXSceneDisplayView* sceneView;
@property (weak) IBOutlet VSCOSXEnvironmentInspectorView* environmentInspectorView;

@property (strong) VSCOSXOBSceneElementInspectorWindowController* elementInspectorWindowController;

-(void) showElementInspectorForElement:(VSC::OB::Scene::Element::SPtr)element;

-(void) reloadInterface;

- (IBAction)test1:(id)sender;
- (IBAction)test2:(id)sender;
- (IBAction)test3:(id)sender;

@end
