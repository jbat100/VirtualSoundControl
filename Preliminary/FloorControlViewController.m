//
//  FloorViewController.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 13/05/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "FloorControlViewController.h"


@implementation FloorControlViewController

- (void)awakeFromNib {
	
	NSLog(@"In FloorControlViewController awakeFromNib");
	
	// Set Text field delegates
	id d = self;
	
	[xCoordinateTextField setDelegate:d];
	[yCoordinateTextField setDelegate:d];
	[zCoordinateTextField setDelegate:d];
	[xNormalVectorTextField setDelegate:d];
	[yNormalVectorTextField setDelegate:d];
	[zNormalVectorTextField setDelegate:d];
	
}


@end
