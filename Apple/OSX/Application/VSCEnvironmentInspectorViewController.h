//
//  VSCEnvironmentInspectorViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 12/2/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol VSCOSXEnvironmentController;

@interface VSCEnvironmentInspectorViewController : NSViewController

@property (assign) id<VSCOSXEnvironmentController> environmentController;

@end
