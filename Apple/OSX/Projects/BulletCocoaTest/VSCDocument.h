//
//  VSCDocument.h
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 21/04/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "btBulletDynamicsCommon.h"
#import "VSCOpenGLView.h"
#import "VSCGLProfilingView.h"
#import "VSCDocumentProtocol.h"
#import "GLDebugDrawer.h"
#import "VSCRootApplication.h"


#define VSC_MINIMUM_MOUSE_SENSITIVITY 0.05
#define VSC_MAXIMUM_MOUSE_SENSITIVITY 2.0

#define VSC_MINIMUM_CAMERA_SPEED 0.05
#define VSC_MAXIMUM_CAMERA_SPEED 5.0


@interface VSCDocument : NSDocument <VSCDocumentProtocol> {
	
	VSCRootApplication* rootApplication;
	VSCApplicationType currentApplicationType;
	
	VSCOpenGLView* vscOpenGLView;
	VSCGLProfilingView* vscGLProfilingView;

	GLDebugDrawer sDebugDraw;
	
	NSButton* resetButton;
	NSButton* pauseButton;
	NSSlider* mouseSensitivitySlider;
	NSSlider* cameraSpeedSlider;
	NSPopUpButton* applicationPopUpButton;
	
	double mouseSensitivity;
	double cameraSpeed;
	
}

@property (retain, nonatomic) IBOutlet VSCOpenGLView* vscOpenGLView;
@property (retain, nonatomic) IBOutlet VSCGLProfilingView* vscGLProfilingView;

@property (retain, nonatomic) IBOutlet NSButton* resetButton;
@property (retain, nonatomic) IBOutlet NSButton* pauseButton;
@property (retain, nonatomic) IBOutlet NSSlider* mouseSensitivitySlider;
@property (retain, nonatomic) IBOutlet NSSlider* cameraSpeedSlider;
@property (retain, nonatomic) IBOutlet NSPopUpButton* applicationPopUpButton;

@property (nonatomic, assign) VSCApplicationType currentApplicationType;

-(IBAction) buttonClicked:(id)sender;
-(IBAction) sliderValueChanged:(id)sender;
-(IBAction) popUpButtonAction:(id)sender;

-(VSCRootApplication*) rootApplication;
-(void) createApplicationWithType:(VSCApplicationType)t;

-(void) updatePauseButton;

@end
