//
//  SensorRelay.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SensorRelay.h"
#import "SettingsManager.h"
#import "TouchRelayView.h"
#import "SBJSON.h"
#import "UITouch+JSONPackaging.h"


@implementation SensorRelay

static SensorRelay* sInstance = nil;

@synthesize currentAcceleration, touchRelayView;

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
		
		touchJSONArray = [[NSMutableArray alloc] initWithCapacity:4];
		stateJSONDictionary;
		
		jsonEncoder = [SBJSON new];
		
	}
	
	return self;
	
}

-(void) dealloc {
	
	[jsonEncoder release];
	jsonEncoder = nil;
	
	[super dealloc];
	
}

#pragma mark -
#pragma mark Sensor Packaging Methods

-(void) updateTouchJSONArray {
	
}

-(void) updateStateJSONDictionary {
	
	
}

#pragma mark -
#pragma mark Sensor Relay Methods

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
