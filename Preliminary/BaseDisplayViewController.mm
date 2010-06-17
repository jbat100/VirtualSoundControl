//
//  BaseDisplayViewController.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 13/06/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "BaseDisplayViewController.h"


@implementation BaseDisplayViewController

@synthesize camera, lightSet, glView;

-(void) setup {
	
	camera = new JBAT_Camera();
	lightSet = new JBAT_LightSet();
	
	[renderingButton setState:0];
	[interactionButton setState:0];
	
	[glView setDelegate:self];
	
}


-(IBAction)	lightSetClicked:(id)sender {
	
	if (!lightSetWindowController) {
		lightSetWindowController = [[LightSetWindowController alloc] init];
		[lightSetWindowController setDataSource:self];
	}
	
	[lightSetWindowController showWindow:self];
	
}

-(IBAction) cameraClicked:(id)sender {
	
	if (!cameraControlWindowController) {
		cameraControlWindowController = [[CameraControlWindowController alloc] init];
		[cameraControlWindowController setDelegate:self];
	}
	
	[cameraControlWindowController showWindow:self];
	
}

-(IBAction) renderingClicked:(id)sender {
	
	if (![renderingButton state]) {
		NSLog(@"Stopped rendering");
		[glView stopRenderingBase];
	}
	else {
		NSLog(@"Started rendering");
		[glView startRenderingBase];
		[glView becomeFirstResponder];
	}
	
}



-(IBAction) interactionClicked:(id)sender {
	NSLog(@"Interaction clicked");
}


@end
