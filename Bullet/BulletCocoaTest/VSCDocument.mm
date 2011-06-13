//
//  VSCDocument.m
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 21/04/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCDocument.h"
#import "VSCBaseApplication.h"

#include <string>


@implementation VSCDocument

@synthesize vscOpenGLView, vscGLProfilingView;
@synthesize resetButton, pauseButton, mouseSensitivitySlider, cameraSpeedSlider, applicationPopUpButton;


-(id) init {
	
	if ((self = [super init])) {
		
		mouseSensitivity = 0.5;
		cameraSpeed = 0.5;
		
		[self createApplicationWithType:VSCApplicationTypeBasic];
		
	}
	
	return self;
	
}

-(void) dealloc {
	
	vscOpenGLView.delegate = nil;
	self.vscOpenGLView = nil;
	if (rootApplication)
		delete rootApplication;
	[super dealloc];
}

#pragma mark - Document Interface and Data Methods

- (NSString *)windowNibName {
    // Implement this to return a nib to load OR implement -makeWindowControllers to manually create your controllers.
    return @"VSCDocument";
}

- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError
{
    // Insert code here to write your document to data of the specified type. If the given outError != NULL, ensure that you set *outError when returning nil.
    // You can also choose to override -fileWrapperOfType:error:, -writeToURL:ofType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.
    // For applications targeted for Panther or earlier systems, you should use the deprecated API -dataRepresentationOfType:. In this case you can also choose to override -fileWrapperRepresentationOfType: or -writeToFile:ofType: instead.

    return nil;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError
{
    // Insert code here to read your document from the given data of the specified type.  If the given outError != NULL, ensure that you set *outError when returning NO.
    // You can also choose to override -readFromFileWrapper:ofType:error: or -readFromURL:ofType:error: instead. 
    // For applications targeted for Panther or earlier systems, you should use the deprecated API -loadDataRepresentation:ofType. In this case you can also choose to override -readFromFile:ofType: or -loadFileWrapperRepresentation:ofType: instead.
    
    return YES;
}

- (void)windowControllerDidLoadNib:(NSWindowController *)windowController {
	
	[mouseSensitivitySlider setMinValue:VSC_MINIMUM_MOUSE_SENSITIVITY];
	[mouseSensitivitySlider setMaxValue:VSC_MAXIMUM_MOUSE_SENSITIVITY];
	[mouseSensitivitySlider setDoubleValue:mouseSensitivity];
	
	[cameraSpeedSlider setMinValue:VSC_MINIMUM_CAMERA_SPEED];
	[cameraSpeedSlider setMaxValue:VSC_MAXIMUM_CAMERA_SPEED];
	[cameraSpeedSlider setDoubleValue:cameraSpeed];
	
}

#pragma mark - VSC Application

-(void) createApplicationWithType:(VSCApplicationType)t {
	
	if (rootApplication)
		delete rootApplication;
	
	rootApplication = NULL;
	
	if (VSCApplicationTypeBasic) {
		rootApplication = new VSCBaseApplication();
		rootApplication->initPhysics();
		//baseApplication->getDynamicsWorld()->setDebugDrawer(&sDebugDraw);
	}
	
	rootApplication->setCameraSpeed((float)cameraSpeed);
	rootApplication->setCameraMouseSensitivity((float)mouseSensitivity);
	
	[self updatePauseButton];
}

#pragma mark - VSCOpenGLViewDelegate Methods

-(VSCRootApplication*) rootApplication {
	return rootApplication;
}

-(void) drewGL {
	
	if (rootApplication) {
		//std::string profileString = rootApplication->profileInfoString();
		//NSString* nsProfileString = [NSString stringWithCString:profileString.c_str()];
	}
	
}

#pragma mark - Interface Methods

-(void) updatePauseButton {
	
	if (rootApplication) {
		if (rootApplication->isIdle()) {
			[pauseButton setTitle:@"Simulate"];
		}
		else {
			[pauseButton setTitle:@"Pause"];
		}

	}
	else {
		[pauseButton setTitle:@"No App"];
	}
	
}

#pragma mark - Interface Callbacks

-(IBAction) buttonClicked:(id)sender {
	
	if (rootApplication) {
		if (sender == resetButton)
			rootApplication->clientResetScene();
		if (sender == pauseButton) {
			rootApplication->setIdle(!rootApplication->isIdle());
		}
	}
	
	if (sender == pauseButton) {
		[self updatePauseButton];
	}
}

-(IBAction) sliderValueChanged:(id)sender {
	
	if (rootApplication) {
		if (sender == cameraSpeedSlider) {
			cameraSpeed = [cameraSpeedSlider doubleValue];
			rootApplication->setCameraSpeed((float)cameraSpeed);
		}
		else if (sender == mouseSensitivitySlider) {
			mouseSensitivity = [mouseSensitivitySlider doubleValue];
			rootApplication->setCameraMouseSensitivity((float)mouseSensitivity);
		}
	}
	
}

-(IBAction) popUpButtonAction:(id)sender {
	
}

@end
