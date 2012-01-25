//
//  CameraControlWindowController.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 11/04/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "CameraControlWindowController.h"

#define MIN_SPEED 0.05
#define MAX_SPEED 1.00

#define MIN_SENSITIVITY 0.0005
#define MAX_SENSITIVITY 0.02


@implementation CameraControlWindowController

@synthesize dataSource;

-(id) init {
	
	if ([super initWithWindowNibName:@"CameraControlWindow"]) {
		[(NSPanel*)[self window] setFloatingPanel:NO];
		[speedSlider setMinValue:(double)MIN_SPEED];
		[speedSlider setMaxValue:(double)MAX_SPEED];
		[mouseSensitivitySlider setMinValue:(double)MIN_SENSITIVITY];
		[mouseSensitivitySlider setMaxValue:(double)MAX_SENSITIVITY];
	}
	return self;
}


-(void) updateTextFields {
	
	if ([dataSource camera]) {
	
		float* pos = [dataSource camera]->getOrigin();
		
		[positionXTextField setStringValue:[NSString stringWithFormat:@"%f", pos[0]]];
		[positionYTextField setStringValue:[NSString stringWithFormat:@"%f", pos[1]]];
		[positionZTextField setStringValue:[NSString stringWithFormat:@"%f", pos[2]]];
		
		float* forward = [dataSource camera]->getForward();
		
		[forwardXTextField setStringValue:[NSString stringWithFormat:@"%f", forward[0]]];
		[forwardYTextField setStringValue:[NSString stringWithFormat:@"%f", forward[1]]];
		[forwardZTextField setStringValue:[NSString stringWithFormat:@"%f", forward[2]]];

		float* up = [dataSource camera]->getUp();
		
		[upXTextField setStringValue:[NSString stringWithFormat:@"%f", up[0]]];
		[upYTextField setStringValue:[NSString stringWithFormat:@"%f", up[1]]];
		[upZTextField setStringValue:[NSString stringWithFormat:@"%f", up[2]]];
		
		[thetaTextField setStringValue:[NSString stringWithFormat:@"%f", [dataSource camera]->getTheta()]];
		[phiTextField setStringValue:[NSString stringWithFormat:@"%f", [dataSource camera]->getPhi()]];
		
	}
	
	else {
		[positionXTextField setStringValue:@""];
		[positionYTextField setStringValue:@""];
		[positionZTextField setStringValue:@""];
		[forwardXTextField setStringValue:@""];
		[forwardYTextField setStringValue:@""];
		[forwardZTextField setStringValue:@""];
		[upXTextField setStringValue:@""];
		[upYTextField setStringValue:@""];
		[upZTextField setStringValue:@""];
		[thetaTextField setStringValue:@""];
		[phiTextField setStringValue:@""];
	}

}

-(void) updateInteractionSliders {
	
	[mouseSensitivitySlider setFloatValue:[dataSource camera]->getRotationSpeed()];
	[speedSlider setFloatValue:[dataSource camera]->getMovementSpeed()];
}

-(IBAction) resetClicked:(id)sender {
	
	[dataSource camera]->setToDefault();
	[dataSource cameraFrameChanged];
	[self updateInteractionSliders];
	
}

-(IBAction) lookAtOriginClicked:(id)sender {
	
}

-(IBAction) sweepClicked:(id)sender {
	
}

-(IBAction) speedSliderChanged:(id)sender {
	[dataSource camera]->setMovementSpeed([speedSlider floatValue]);
}

-(IBAction) mouseSensitivitySliderChanged:(id)sender {
	[dataSource camera]->setRotationSpeed([mouseSensitivitySlider floatValue]);
}


@end
