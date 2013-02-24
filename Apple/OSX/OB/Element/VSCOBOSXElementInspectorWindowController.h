//
//  VSCElementInspectorWindowController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCOBOSXElementController.h"
#import "VSCOBOSXSceneListener.h"

@class VSCOBOSXElementInspectorViewController;


@interface VSCOBOSXElementInspectorWindowController : NSWindowController <NSWindowDelegate>

@property (strong) VSCOBOSXElementInspectorViewController* elementInspectorViewController;

@end
