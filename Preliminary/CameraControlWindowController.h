//
//  CameraControlWindowController.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 11/04/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "JBAT_Camera.h"

@protocol CameraControlWindowControllerDataSource

-(JBAT_Camera*) camera;

@end



@interface CameraControlWindowController : NSWindowController {
	
	NSPopUpButton *cameraTypeButton;

	IBOutlet NSTextField *positionXTextField;
	IBOutlet NSTextField *positionYTextField;
	IBOutlet NSTextField *positionZTextField;
	
	IBOutlet NSTextField *forwardXTextField;
	IBOutlet NSTextField *forwardYTextField;
	IBOutlet NSTextField *forwardZTextField;
	
	IBOutlet NSTextField *upXTextField;
	IBOutlet NSTextField *upYTextField;
	IBOutlet NSTextField *upZTextField;
	
	IBOutlet NSTextField *phiTextField;
	IBOutlet NSTextField *thetaTextField;
	
	IBOutlet NSButton *resetButton;
	IBOutlet NSButton *lookAtOriginButton;
	IBOutlet NSButton *sweeptButton;
	
	id<CameraControlWindowControllerDataSource> dataSource;
	
}

@property (assign) id<CameraControlWindowControllerDataSource> dataSource;

-(void) updateTextFields;
-(IBAction) resetClicked:(id)sender;
-(IBAction) lookAtOriginClicked:(id)sender;
-(IBAction) sweepClicked:(id)sender;


@end
