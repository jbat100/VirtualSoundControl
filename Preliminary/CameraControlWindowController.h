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
-(void) cameraFrameChanged;

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
	
	IBOutlet NSSlider *speedSlider;
	IBOutlet NSSlider *mouseSensitivitySlider;
	
	id<CameraControlWindowControllerDataSource> dataSource;
	
}

@property (assign) id<CameraControlWindowControllerDataSource> dataSource;

-(void) updateTextFields;
-(void) updateInteractionSliders;

-(IBAction) resetClicked:(id)sender;
-(IBAction) lookAtOriginClicked:(id)sender;
-(IBAction) sweepClicked:(id)sender;

-(IBAction) speedSliderChanged:(id)sender;
-(IBAction) mouseSensitivitySliderChanged:(id)sender;



@end
