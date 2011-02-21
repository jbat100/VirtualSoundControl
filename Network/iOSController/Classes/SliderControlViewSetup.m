//
//  SliderControlViewSetup.m
//  iOSController
//
//  Created by Jonathan Thorpe on 16/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SliderControlViewSetup.h"


@implementation SliderControlViewSetup


@synthesize sliderRanges, sliderValues, numberOfSlider, sliderOrientation;

-(id) init {
	
	if ((self = [super init])) {
		sliderOrientation = SliderOrientationHorizontal;
		sliderSetups = [[NSMutableArray alloc] initWithCapacity:10];
	}
	
	return self;
	
}

-(void) encodeWithCoder:(NSCoder *)coder {
	
	[super encodeWithCoder:coder];
	
	[coder encodeInteger:(NSInteger)sliderOrientation forKey:@"SliderControlViewSetupSliderOrientation"];
	[coder encodeObject:sliderSetups forKey:@"SliderControlViewSetupSliderSetups"];
	
}

-(id) initWithCoder:(NSCoder *)coder {
	
	if ((self = [super initWithCoder:coder])) {
		
		self.sliderOrientation = (SliderOrientation)[coder decodeCGRectForKey:@"SliderControlViewSetupSliderOrientation"];
		self.sliderSetups = [coder decodeObjectForKey:@"SliderControlViewSetupSliderSetups"];
		
	}
	
	return self;
	
}


@end
