//
//  SensorRelay.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "ControlStation.h"
#import "ServerController.h"
#import "SettingsManager.h"
#import "ControlView.h"

#import "TouchControlView.h"
#import "SBJSON.h"
#import "UITouch+JSONPackaging.h"
#import "NetworkDefinitions.h"

@interface ControlStation () 

-(void) sendCurrentState;
-(void) updateStateJSONDictionary;

@property (nonatomic, retain) UIAcceleration* currentAcceleration;

@end


@implementation ControlStation

static ControlStation* sInstance = nil;

@synthesize updateInterval, currentAcceleration;

+(ControlStation*) instance {
	
	@synchronized (self) {
		
		if (!sInstance) 
			sInstance = [[ControlStation alloc] init];
		
	}
	
	return sInstance;
	
}

-(id) init {
	
	if ((self = [super init])) {
		
		self.updateInterval = 0.1;
		
		controlViews = [[NSMutableArray alloc] initWithCapacity:4];
		
		[UIAccelerometer sharedAccelerometer].delegate = self;
		[UIAccelerometer sharedAccelerometer].updateInterval = self.updateInterval;
		
		stateJSONDictionary = [[NSMutableDictionary alloc] initWithCapacity:4];
		
		NSMutableArray* controlViewsJSONArray = [[NSMutableArray alloc] initWithCapacity:4];
		[stateJSONDictionary setObject:controlViewsJSONArray forKey:
		 [NSString stringWithCString:JSON_CTRL_VIEW_ARRAY_KEY encoding:NSUTF8StringEncoding]];
		[controlViewsJSONArray release];
		
		NSMutableDictionary* accelerationJSONDictionary = [[NSMutableDictionary alloc] initWithCapacity:3];
		[stateJSONDictionary setObject:accelerationJSONDictionary forKey:
		 [NSString stringWithCString:JSON_ACCELERATION_DICTIONARY_KEY encoding:NSUTF8StringEncoding]];
		[accelerationJSONDictionary release];
		
		jsonEncoder = [[SBJSON new] autorelease];
		
	}
	
	return self;
	
}

-(void) dealloc {
	
	[controlViews release];
	[stateJSONDictionary release];
	[jsonEncoder release];
	
	[super dealloc];
	
}

#pragma mark -
#pragma mark Sensor Packaging Methods


-(void) updateStateJSONDictionary {
	
	
	
	// UPDATE TOUCH JSON DESCRIPTION FOR ALL VIEWS IN THE TOUCHRELAYVIEWS DICTIONARY
	
	// Get and empty the json representation
	NSMutableArray* controlViewsJSONArray = [stateJSONDictionary objectForKey:
											 [NSString stringWithCString:JSON_CTRL_VIEW_ARRAY_KEY 
																encoding:NSUTF8StringEncoding]];
	
	[controlViewsJSONArray removeAllObjects];

	
	for (ControlView* controlView in controlViews) {
		
		[controlView updateJSONDescriptionDictionary];
		[controlViewsJSONArray addObject:controlView.jsonDescriptionDictionary];
		
	}
	
	// UPDATE ACCELERATION DATA
	
	NSMutableDictionary* accelerationJSONDictionary = [stateJSONDictionary objectForKey:
								   [NSString stringWithCString:JSON_ACCELERATION_DICTIONARY_KEY encoding:NSUTF8StringEncoding]];
	[accelerationJSONDictionary setObject:[NSString stringWithFormat:@"%.4f", currentAcceleration.x] 
								   forKey:[NSString stringWithCString:JSON_ACCELERATION_X_KEY encoding:NSUTF8StringEncoding]];
	[accelerationJSONDictionary setObject:[NSString stringWithFormat:@"%.4f", currentAcceleration.y] 
								   forKey:[NSString stringWithCString:JSON_ACCELERATION_Y_KEY encoding:NSUTF8StringEncoding]];
	[accelerationJSONDictionary setObject:[NSString stringWithFormat:@"%.4f", currentAcceleration.z] 
								   forKey:[NSString stringWithCString:JSON_ACCELERATION_Z_KEY encoding:NSUTF8StringEncoding]];
	
	
}

#pragma mark -
#pragma mark Sensor Relay Methods

-(void) startBroadcasting {
	
	if (!relayTimer) {
		
		relayTimer = [[NSTimer scheduledTimerWithTimeInterval:updateInterval 
													   target:self 
													 selector:@selector(sendCurrentState) 
													 userInfo:nil 
													  repeats:YES] retain];
		
	}

}

-(void) stopBroadcasting {
	
	[relayTimer invalidate];
	relayTimer = nil;
	
}


-(void) sendCurrentState {
	
	//NSLog(@"Sending current state");
	
	[self updateStateJSONDictionary];
	
	NSString* jsonString = [jsonEncoder stringWithObject:stateJSONDictionary];
	
	NSLog(@"Sending JSON State: %@", jsonString);
	
	NSData* jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
	
	[[ServerController instance] sendData:jsonData];
	
}

#pragma mark -
#pragma mark TouchRelayViews Methods

-(void) addControlView:(ControlView*)relayView  {
	[controlViews addObject:relayView];
}

-(void) removeControlView:(ControlView*)relayView  {
	[controlViews removeObjectIdenticalTo:relayView];
}

-(void) removeAllTouchRelayViews {
	[controlViews removeAllObjects];
}

#pragma mark -
#pragma mark UIAccelerometer Delegate Methods

- (void)accelerometer:(UIAccelerometer *)acel didAccelerate:(UIAcceleration *)aceler {
	[currentAcceleration release];
	currentAcceleration = [aceler retain];
}

#pragma mark -
#pragma mark Custom Setters 

-(void) setUpdateInterval:(NSTimeInterval)i {
	
	if (i < MINIMUM_UPDATE_INTERVAL) 
		updateInterval = MINIMUM_UPDATE_INTERVAL;
	else 
		updateInterval = i;
	
	[UIAccelerometer sharedAccelerometer].updateInterval = updateInterval;
	
	// Restart relaying so that the timer gets reinitialised with new time interval;
	[self stopBroadcasting];
	[self startBroadcasting];
	
}


@end
