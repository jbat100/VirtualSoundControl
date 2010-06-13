//
//  MeshDisplayView.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 28/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "MeshDisplayViewController.h"
#import "JBAT_Camera.h"


@implementation MeshDisplayViewController

@synthesize camera, glView, dataSource;

-(void) awakeFromNib {
	
	NSRect frame = self.view.frame;
	frame.origin.x = frame.origin.y = 0;
	
	//glView = [[BaseGLView alloc] initWithFrame:frame];
	
	NSLog(@"In MeshDisplayViewController glView is %@", glView);
	
	camera = new JBAT_Camera();
	
	NSLog(@"In MeshDisplayViewController awakeFromNib");
	NSLog(@"glView is %@", glView);
	NSLog(@"glView rendering context is %@", [glView openGLContext]);
	
}

-(void) setup {
	
}

@end
