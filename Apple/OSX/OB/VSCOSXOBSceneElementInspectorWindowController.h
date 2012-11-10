//
//  VSCSceneElementInspectorWindowController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VSCOSXOBSceneElementInspectorView;

@interface VSCOSXOBSceneElementInspectorWindowController : NSWindowController

@property (weak) IBOutlet VSCOSXOBSceneElementInspectorView* elementInspectorView;

@end
