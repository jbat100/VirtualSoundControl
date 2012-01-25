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
		
		self.sliderOrientation = (SliderOrientation)[coder decodeIntegerForKey:@"SliderControlViewSetupSliderOrientation"];
		self.sliderSetups = [coder decodeObjectForKey:@"SliderControlViewSetupSliderSetups"];
		
	}
	
	return self;
	
}

-(NSArray*) channels {
	
	NSMutableArray* chans = [[[NSMutableArray alloc] initWithCapacity:[sliderSetups count]] autorelease];
	
	for (SliderSetup* sliderSetup in sliderSetups) 
		[chans addObject:[NSNumber numberWithInteger:sliderSetup.channel]];
	
	return [NSArray arrayWithArray:chans];
}

-(void) setSliderSetup:(SliderSetup*)prototypeSetup forChannels:(NSArray*)channels {
	
	[sliderSetups removeAllObjects];
	
	for (NSNumber* channel in channels) {
		if ([channel isKindOfClass:[NSNumber class]])
			[NSException raise:@"Invalid channels" format:@"%@ should be an array of NSNumber int objects", channels];
		NSInteger chan = [channel intValue];
		SliderSetup* sliderSetup = [SliderSetup sliderSetupWithSliderSetup:prototypeSetup];
		sliderSetup.channel = chan;
		[sliderSetups addObject:sliderSetup];
	}
	
}




@end
