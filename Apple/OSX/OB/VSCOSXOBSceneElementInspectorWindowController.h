//
//  VSCSceneElementInspectorWindowController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCOSXOBSceneElementController.h"

@class VSCOSXOBSceneElementInspectorView;

/*
 *  Main element inspector window;
 */

@interface VSCOSXOBSceneElementInspectorWindowController : NSWindowController <VSCOSXOBSceneElementController>

@property (weak) IBOutlet VSCOSXOBSceneElementInspectorView* elementInspectorView;

@end
