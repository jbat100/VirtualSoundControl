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
	//NSLog(@"In ViewComponentsControlWindowController init");
	if ([super initWithWindowNibName:@"ViewComponentsControlWindow"]) {
		//NSLog(@"Created ViewComponentsControlWindowController");
		lightSet = new JBAT_LightSet();
		//std::cout << viewComponents;
		
	}
	return self;
}

-(void) windowDidLoad {
	
	//NSLog(@"In ViewComponentsControlWindowController windowDidLoad");
	
	viewComponentsControlViewController = 
	[[ViewComponentsControlViewController alloc] initWithNibName:@"ViewComponentsControlView" 
														  bundle:[NSBundle mainBundle]];
	
	[self.window setContentView:viewComponentsControlViewController.view];
	
	[[viewComponentsControlViewController lightsControlViewController] setDataSource:self];
	
	[viewComponentsControlViewController setup];
	[viewComponentsControlViewController updateInterface];
	
}

@end
