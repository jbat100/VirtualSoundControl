//
//  VSCOSXOBSceneElementDetailView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol VSCOSXOBSceneElementController;

@interface VSCOSXOBSceneElementDetailView : NSView

@property (weak) id<VSCOSXOBSceneElementController> elementController;

@property (weak) IBOutlet NSTextField* nameTextField;
@property (weak) IBOutlet NSTextField* idTextField;

/*
 *  Position/Rotation
 */

@property (weak) IBOutlet NSTextField* xPosTextField;
@property (weak) IBOutlet NSTextField* yPosTextField;
@property (weak) IBOutlet NSTextField* zPosTextField;

@property (weak) IBOutlet NSTextField* wRotTextField;
@property (weak) IBOutlet NSTextField* xRotTextField;
@property (weak) IBOutlet NSTextField* yRotTextField;
@property (weak) IBOutlet NSTextField* zRotTextField;

-(void) reloadInterface;

@end
