//
//  CameraControlWindowController.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 11/04/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "CameraControlWindowController.h"


@implementation CameraControlWindowController

@synthesize dataSource;

-(id) init {
	
	if ([super initWithWindowNibName:@"CameraControlWindow"]) {
		
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

-(IBAction) resetClicked:(id)sender {
	
}

-(IBAction) lookAtOriginClicked:(id)sender {
	
}

-(IBAction) sweepClicked:(id)sender {
	
}


@end
