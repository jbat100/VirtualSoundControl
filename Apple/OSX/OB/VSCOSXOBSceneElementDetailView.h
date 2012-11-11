//
//  VSCOSXOBSceneElementDetailView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface VSCOSXOBSceneElementDetailView : NSView

@property (weak) IBOutlet NSTextField* nameTextField;

/*
 *  Position/Rotation
 */

@property (weak) IBOutlet NSTextField* xPosTextField;
@property (weak) IBOutlet NSTextField* yPosTextField;
@property (weak) IBOutlet NSTextField* zPosTextField;

@property (weak) IBOutlet NSTextField* rollTextField;
@property (weak) IBOutlet NSTextField* pitchTextField;
@property (weak) IBOutlet NSTextField* yawTextField;

@end
