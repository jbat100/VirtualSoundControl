//
//  VSCOSXOBSceneElementDetailView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface VSCOSXOBSceneElementDetailView : NSView

@property (nonatomic, strong) IBOutlet NSTextField* nameTextField;

/*
 *  Position/Rotation
 */

@property (nonatomic, strong) IBOutlet NSTextField* xPosTextField;
@property (nonatomic, strong) IBOutlet NSTextField* yPosTextField;
@property (nonatomic, strong) IBOutlet NSTextField* zPosTextField;

@property (nonatomic, strong) IBOutlet NSTextField* rollTextField;
@property (nonatomic, strong) IBOutlet NSTextField* pitchTextField;
@property (nonatomic, strong) IBOutlet NSTextField* yawTextField;

@end
