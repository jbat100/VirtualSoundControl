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

@class VSCOSXOBSceneElementInspectorView;

/*
 *  Main element inspector window;
 *  Listens to the element scene to be able to update element pos/vel/rot on
 *  scene renders.
 */

@interface VSCOSXOBSceneElementInspectorWindowController : NSWindowController
<VSCOSXOBSceneElementController, VSCOBOSXSceneListenerTarget>

@property (weak) IBOutlet VSCOSXOBSceneElementInspectorView* elementInspectorView;

@property (assign) VSC::OB::OSXSceneListener::SPtr sceneListener;

@end
