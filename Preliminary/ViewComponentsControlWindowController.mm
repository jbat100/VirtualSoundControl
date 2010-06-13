//
//  ViewComponentsControlWindowController.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 05/04/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "ViewComponentsControlWindowController.h"
#import <iostream>


@implementation ViewComponentsControlWindowController

@synthesize lightSet;

-(id) init {

	if ([super initWithWindowNibName:@"ViewComponentsControlWindow"]) {
		lightSet = new JBAT_LightSet();
	}
	return self;
}

-(void) windowDidLoad {
	

	viewComponentsControlViewController = 
	[[ViewComponentsControlViewController alloc] initWithNibName:@"ViewComponentsControlView" 
														  bundle:[NSBundle mainBundle]];
	
	[self.window setContentView:viewComponentsControlViewController.view];
	
	[[viewComponentsControlViewController lightsControlViewController] setDataSource:self];
	
	[viewComponentsControlViewController setup];
	[viewComponentsControlViewController updateInterface];
	
}

@end
