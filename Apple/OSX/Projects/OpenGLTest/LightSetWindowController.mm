//
//  LightSetWindowController.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 17/06/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "LightSetWindowController.h"


@implementation LightSetWindowController

@synthesize dataSource;

-(id) init {
	
	if ([super initWithWindowNibName:@"LightSetWindow"]) {

	}
	return self;
}

-(void) windowDidLoad {
	
	
	lightsControlViewController = [[LightsControlViewController alloc] initWithNibName:@"LightsControlView" 
																				bundle:[NSBundle mainBundle]];
	
	
	NSRect f = lightSetView.frame;
	f.origin.x = f.origin.y = 0;
	lightsControlViewController.view.frame = f;
	[lightsControlViewController setDataSource:self];
	[lightSetView addSubview:[lightsControlViewController view]];
	
	[lightsControlViewController setup];
	[lightsControlViewController updateInterface];
	
}

-(JBAT_LightSet*) lightSet {
	
	return [dataSource lightSet];
	
}



@end
