//
//  SliderSetup.m
//  iOSController
//
//  Created by Jonathan Thorpe on 22/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SliderSetup.h"


@implementation SliderSetup

@synthesize value, minValue, maxValue, channel;

+(SliderSetup*) sliderSetupWithSliderSetup:(SliderSetup*)sliderSetup {
	
	SliderSetup* copiedSlider = [[[SliderSetup alloc] init] autorelease];
	
	copiedSlider.value = sliderSetup.value;
	copiedSlider.minValue = sliderSetup.minValue;
	copiedSlider.maxValue = sliderSetup.maxValue;
	copiedSlider.channel = sliderSetup.channel;
	
}

-(id) init {
	
	if ((self = [super init])) {
		value = 0.0;
		minValue = 0.0;
		maxValue = 127.0;
		channel = 0;
	}
	
	return self;
	
}

-(void) encodeWithCoder:(NSCoder *)coder {
	
	[coder encodeFloat:value forKey:@"SliderSetupValue"];
	[coder encodeFloat:minValue forKey:@"SliderSetupMinValue"];
	[coder encodeFloat:maxValue forKey:@"SliderSetupMaxValue"];
	[coder encodeInteger:channel forKey:@"SliderSetupChannel"];
	
}

-(id) initWithCoder:(NSCoder *)coder {
	
	if ((self = [super init])) {
		
		value = [coder decodeFloatForKey:@"SliderSetupValue"];
		minValue = [coder decodeFloatForKey:@"SliderSetupMinValue"];
		maxValue = [coder decodeFloatForKey:@"SliderSetupMaxValue"];
		channel = [coder decodeIntegerForKey:@"SliderSetupChannel"];
		
	}
	
	return self;
	
}

@end
