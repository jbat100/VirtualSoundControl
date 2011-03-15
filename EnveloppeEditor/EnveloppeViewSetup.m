//
//  EnveloppeViewSetup.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "EnveloppeViewSetup.h"
#import "CGColorOperations.h"

@implementation EnveloppeViewSetup

@synthesize minTime, maxTime, minValue, maxValue, dB;
@synthesize unselectedControlPointColor, selectedControlPointColor;

-(id) init {
	
	if ((self == [super init])) {
		minTime = 0.0;
		maxTime = 5.0;
		minValue = -1.0;
		maxValue = 1.0;
		dB = NO;
		unselectedControlPointColor = CGColorCreateFromRGBNSColor([NSColor blackColor]);
		unselectedControlPointColor = CGColorCreateFromRGBNSColor([NSColor blueColor]);
	}
	
}

-(id) initWithCoder:(NSCoder *)coder {
	
	if ((self = [super init])) {
		minTime = [coder decodeDoubleForKey:@"EnveloppeViewSetupMinTime"];
		maxTime = [coder decodeDoubleForKey:@"EnveloppeViewSetupMaxTime"];
		minValue = [coder decodeDoubleForKey:@"EnveloppeViewSetupMinValue"];
		maxValue = [coder decodeDoubleForKey:@"EnveloppeViewSetupMaxValue"];
		dB = [coder decodeBoolForKey:@"EnveloppeViewSetupDB"];
	}
	return self;
}

-(void) encodeWithCoder:(NSCoder *)coder {
	
	[coder encodeDouble:minTime forKey:@"EnveloppeViewSetupMinTime"];
	[coder encodeDouble:maxTime forKey:@"EnveloppeViewSetupMaxTime"];
	[coder encodeDouble:minValue forKey:@"EnveloppeViewSetupMinValue"];
	[coder encodeDouble:maxValue forKey:@"EnveloppeViewSetupMaxValue"];
	[coder encodeBool:dB forKey:@"EnveloppeViewSetupDB"];
	
}


@end
