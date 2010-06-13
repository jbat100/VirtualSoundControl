//
//  BaseDisplayViewController.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 13/06/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "BaseDisplayViewController.h"


@implementation BaseDisplayViewController

@synthesize camera, glView;

-(void) setup {
	
	camera = new JBAT_Camera();
	
	[renderingButton setState:0];
	[interactionButton setState:0];
	
}


-(IBAction)	viewComponentsClicked:(id)sender {
	
	if (!viewComponentsControlWindowController) {
		viewComponentsControlWindowController = [[ViewComponentsControlWindowController alloc] init];
	}
	
	[viewComponentsControlWindowController showWindow:self];
	
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


-(IBAction) cameraClicked:(id)sender {
	NSLog(@"Camera clicked");
}

-(IBAction) interactionClicked:(id)sender {
	NSLog(@"Interaction clicked");
}


@end
