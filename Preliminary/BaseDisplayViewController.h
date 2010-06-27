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
#import "JBAT_BufferedMesh.h"

#import "CameraControlWindowController.h"
#import "LightSetWindowController.h"


@interface BaseDisplayViewController : NSViewController 

<BaseGLViewEnvironment,
CameraControlWindowControllerDataSource,
LightSetWindowControllerDataSource> 

{

	IBOutlet BaseGLView *glView;
	
	IBOutlet NSButton* viewComponentsButton;
	IBOutlet NSButton* cameraButton;
	IBOutlet NSButton* interactionButton;
	IBOutlet NSButton* renderingButton;
	
	IBOutlet NSPopUpButton* meshDisplayModePopUpButton;
	
	CameraControlWindowController *cameraControlWindowController;
	LightSetWindowController *lightSetWindowController;
	
	JBAT_Camera *camera;
	JBAT_LightSet *lightSet;
	JBAT_BufferedMesh *floor;
	
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
@property (assign) JBAT_BufferedMesh *floor;

-(void) setup;
-(void) updateMeshDisplayMode;

-(IBAction)	lightSetClicked:(id)sender;
-(IBAction) renderingClicked:(id)sender;
-(IBAction) cameraClicked:(id)sender;
-(IBAction) interactionClicked:(id)sender;

-(void) startRendering;
-(void) stopRendering;

- (void)keyDown:(NSEvent *)theEvent;

@end