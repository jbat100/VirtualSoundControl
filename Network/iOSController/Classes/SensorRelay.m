//
//  SensorRelay.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SensorRelay.h"
#import "SettingsManager.h"



@implementation SensorRelay

static SensorRelay* sInstance = nil;

@synthesize currentAcceleration;

+(SensorRelay*) instance {
	
	@synchronized (self) {
		
		if (!sInstance) 
			sInstance = [[SensorRelay alloc] init];
		
	}
	
	return sInstance;
	
}

-(id) init {
	
	if ((self = [super init])) {
		[UIAccelerometer sharedAccelerometer].delegate = self;
		[UIAccelerometer sharedAccelerometer].updateInterval = [SettingsManager instance].updateInterval;
	}
	
	return self;
	
}


- (void)accelerometer:(UIAccelerometer *)acel didAccelerate:(UIAcceleration *)aceler {
	
	self.currentAcceleration = aceler;
	
}

@end
