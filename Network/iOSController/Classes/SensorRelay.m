//
//  SensorRelay.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SensorRelay.h"
#import "SettingsManager.h"
#import "JSON.h"



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


-(void) startRelaying {
	
	if (!relayTimer) {
		
		NSTimeInterval updateInterval = [SettingsManager instance].updateInterval;
		
		relayTimer = [[NSTimer scheduledTimerWithTimeInterval:updateInterval 
													   target:self 
													 selector:@selector(sendCurrentState) 
													 userInfo:nil 
													  repeats:YES] retain];
		
	}

}

-(void) stopRelaying {
	
	[relayTimer invalidate];
	
	relayTimer = nil;
	
}


-(void) sendCurrentState {
	
	NSLog(@"Sending current state");
	
}

#pragma mark -
#pragma mark UIAccelerometer Delegate Methods

- (void)accelerometer:(UIAccelerometer *)acel didAccelerate:(UIAcceleration *)aceler {
	self.currentAcceleration = aceler;
}

@end
