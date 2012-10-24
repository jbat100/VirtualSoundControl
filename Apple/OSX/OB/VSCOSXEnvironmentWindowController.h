//
//  VSCOBOSXSceneWindow.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCOSXApplicationManagerProtocol.h"

#include "VSCEnvironment.h"

@class VSCOBOSXSceneDisplayView;

@interface VSCOSXEnvironmentWindowController : NSWindowController

@property (nonatomic, strong) IBOutlet VSCOBOSXSceneDisplayView* sceneView;

@property (nonatomic, assign) VSC::Environment::SPtr environment;

-(void) reloadInterface;

- (IBAction)test1:(id)sender;
- (IBAction)test2:(id)sender;
- (IBAction)test3:(id)sender;

@end
