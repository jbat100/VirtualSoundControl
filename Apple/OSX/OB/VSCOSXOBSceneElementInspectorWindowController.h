//
//  VSCSceneElementInspectorWindowController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCOSXOBSceneElementController.h"
#import "VSCOBOSXSceneListener.h"

@class VSCOSXOBSceneElementInspectorViewController;


@interface VSCOSXOBSceneElementInspectorWindowController : NSWindowController

@property (strong) VSCOSXOBSceneElementInspectorViewController* elementInspectorViewController;

@property (weak) IBOutlet NSBox* mainBox;

@end
