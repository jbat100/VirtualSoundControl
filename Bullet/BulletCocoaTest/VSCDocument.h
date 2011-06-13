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

#include "GLDebugDrawer.h"
class VSCBaseApplication;


#define VSC_MINIMUM_MOUSE_SENSITIVITY 0.05
#define VSC_MAXIMUM_MOUSE_SENSITIVITY 2.0

#define VSC_MINIMUM_CAMERA_SPEED 0.05
#define VSC_MAXIMUM_CAMERA_SPEED 5.0

enum VSCApplicationType {
	VSCApplicationTypeNone = 0,
	VSCApplicationTypeBasic
};

@interface VSCDocument : NSDocument <VSCOpenGLViewDelegate> {
	
	VSCOpenGLView* vscOpenGLView;
	VSCRootApplication* rootApplication;

	GLDebugDrawer sDebugDraw;
	
	NSButton* resetButton;
	NSSlider* mouseSensitivitySlider;
	NSSlider* cameraSpeedSlider;
	NSPopUpButton* applicationPopUpButton;
	
	double mouseSensitivity;
	double cameraSpeed;
	
}

@property (retain, nonatomic) IBOutlet VSCOpenGLView* vscOpenGLView;

@property (retain, nonatomic) IBOutlet NSButton* resetButton;
@property (retain, nonatomic) IBOutlet NSSlider* mouseSensitivitySlider;
@property (retain, nonatomic) IBOutlet NSSlider* cameraSpeedSlider;
@property (retain, nonatomic) IBOutlet NSPopUpButton* applicationPopUpButton;

-(IBAction) buttonClicked:(id)sender;
-(IBAction) sliderValueChanged:(id)sender;
-(IBAction) popUpButtonAction:(id)sender;

-(VSCRootApplication*) rootApplication;
-(void) createApplicationWithType:(VSCApplicationType)t;

@end
