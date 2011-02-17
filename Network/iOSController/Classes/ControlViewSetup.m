//
//  ControlViewSetup.m
//  iOSController
//
//  Created by Jonathan Thorpe on 13/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "ControlViewSetup.h"


@implementation ControlViewSetup


@synthesize normalisedFrame, descriptor, backgroundImageName, view;

-(void) dealloc {
	
	[descriptor release];
	[backgroundImageName release];
	[view release];
	
	[super dealloc];
	
}

@end
