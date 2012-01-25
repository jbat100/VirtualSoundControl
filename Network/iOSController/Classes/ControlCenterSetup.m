//
//  ControlSetup.m
//  iOSController
//
//  Created by Jonathan Thorpe on 09/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "ControlCenterSetup.h"

#import "ControlViewSetup.h"
#import "TouchControlViewSetup.h"
#import "SliderControlViewSetup.h"
#import "ButtonControlViewSetup.h"

#import "ControlView.h"
#import "TouchControlView.h"
#import "SliderControlView.h"
#import "ButtonControlView.h"

@implementation ControlCenterSetup

@synthesize controlViewSetups, descriptor;

-(id) init {
	
	if ((self = [super init])) {
		
		controlViewSetups = [[NSMutableArray alloc] initWithCapacity:10];
		
	}
	
	return self;
	
}



-(void) applySetupToView:(UIView*)view {
	
	// First remove all subviews
	NSArray* subviews = [view subviews];
	for (UIView* subview in subviews) 
		[subview removeFromSuperview];
	
	CGRect defaultFrame = view.frame;
	defaultFrame.origin.x = defaultFrame.origin.y = 0;
	
	for (ControlViewSetup* viewSetup in controlViewSetups) {
		
		ControlView* controlView = nil;
		
		if ([viewSetup isKindOfClass:[TouchControlViewSetup class]]) 
			controlView = [[TouchControlView alloc] initWithFrame:defaultFrame];
		
		if ([viewSetup isKindOfClass:[SliderControlViewSetup class]]) 
			controlView = [[SliderControlView alloc] initWithFrame:defaultFrame];
		
		if ([viewSetup isKindOfClass:[ButtonControlViewSetup class]]) 
			controlView = [[ButtonControlView alloc] initWithFrame:defaultFrame];
		
		controlView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
		
		[view addSubview:controlView];
		
		controlView.setup = viewSetup;
		viewSetup.view = controlView;
		
		[controlView applySetup];
		
	}
	
}

-(void) dealloc {
	
	[controlViewSetups release];
	
	[super dealloc];
	
}

@end
