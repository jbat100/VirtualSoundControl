//
//  BaseDisplayViewController.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 13/06/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BaseGLView.h"
#import "JBAT_Camera.h"
#import "JBAT_LightSet.h"
#import "CameraControlWindowController.h"
#import "LightSetWindowController.h"


@interface BaseDisplayViewController : NSViewController 

<GLViewInteractionDelegate,
CameraControlWindowControllerDataSource,
LightSetWindowControllerDataSource> 

{

	IBOutlet BaseGLView *glView;
	
	IBOutlet NSButton* viewComponentsButton;
	IBOutlet NSButton* cameraButton;
	IBOutlet NSButton* interactionButton;
	IBOutlet NSButton* renderingButton;
	
	CameraControlWindowController *cameraControlWindowController;
	LightSetWindowController *lightSetWindowController;
	
	JBAT_Camera *camera;
	JBAT_LightSet *lightSet;
	
	// used to update screen
	NSTimer *renderingTimer;
	NSTimer *interactionTimer;
	
	BOOL interactionEnabled;
	BOOL rendering;
	// KEY TRACKING
	BOOL upIsPressed;
	BOOL downIsPressed;
	BOOL leftIsPressed;
	BOOL rightIsPressed;
	BOOL forwardIsPressed;
	BOOL backwardIsPressed;
		
}

@property (assign) BaseGLView *glView;
@property (assign) JBAT_Camera *camera;
@property (assign) JBAT_LightSet *lightSet;

-(void) setup;

-(IBAction)	lightSetClicked:(id)sender;
-(IBAction) renderingClicked:(id)sender;
-(IBAction) cameraClicked:(id)sender;
-(IBAction) interactionClicked:(id)sender;

-(void) startRenderingBase;
-(void) stopRenderingBase;

- (void)keyDown:(NSEvent *)theEvent;

@end