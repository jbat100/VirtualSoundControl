//
//  BaseDisplayView.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 28/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "ViewComponentsControlViewController.h"

@implementation ViewComponentsControlViewController

@synthesize lightsControlViewController;

#pragma mark -
#pragma mark NSViewController Methods

- (void)awakeFromNib {
	
	NSLog(@"In ViewComponentsControlViewController awakeFromNib");

	
	lightsControlViewController = [[LightsControlViewController alloc] initWithNibName:@"LightsControlView" 
																				bundle:[NSBundle mainBundle]];
	

	[[lightsControlViewController view] setAutoresizesSubviews:YES];
	[[lightsControlViewController view] setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
	
	NSTabViewItem *lightsTabItem1 = [controlTabView tabViewItemAtIndex:0];
	[lightsTabItem1 setView:[lightsControlViewController view]];
	
	//NSLog(@"Numer of lights is %d", [dataSource viewComponents]->numberOfLights());
	//NSTabViewItem *lightsTabItem2 = [controlTabView tabViewItemAtIndex:1];
	
	
}

- (void) setup {
	
	[lightsControlViewController setup];
	
}

- (void) updateInterface {
	
	[lightsControlViewController updateInterface];
	
}

- (void) dealloc {
	
	[lightsControlViewController release];
	[super dealloc];
	
}



@end
