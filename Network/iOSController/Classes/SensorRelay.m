//
//  SensorRelay.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SensorRelay.h"
#import "ServerController.h"
#import "SettingsManager.h"
#import "TouchRelayView.h"
#import "SBJSON.h"
#import "UITouch+JSONPackaging.h"
#import "NetworkDefinitions.h"

@interface SensorRelay () 

-(void) sendCurrentState;
-(void) updateStateJSONDictionary;

@property (nonatomic, retain) NSMutableDictionary* touchRelayViews;
@property (nonatomic, retain) UIAcceleration* currentAcceleration;
@property (nonatomic, retain) NSMutableDictionary* stateJSONDictionary;
@property (nonatomic, retain) SBJSON* jsonEncoder;
@property (nonatomic, retain) NSTimer* relayTimer;

@end


@implementation SensorRelay

static SensorRelay* sInstance = nil;

@synthesize updateInterval;
@synthesize touchRelayViews, currentAcceleration, stateJSONDictionary, jsonEncoder, relayTimer;

+(SensorRelay*) instance {
	
	@synchronized (self) {
		
		if (!sInstance) 
			sInstance = [[SensorRelay alloc] init];
		
	}
	
	return sInstance;
	
}

-(id) init {
	
	if ((self = [super init])) {
		
		self.updateInterval = 0.1;
		
		self.touchRelayViews = [[[NSMutableDictionary alloc] initWithCapacity:4] autorelease];
		
		[UIAccelerometer sharedAccelerometer].delegate = self;
		[UIAccelerometer sharedAccelerometer].updateInterval = self.updateInterval;
		
		self.stateJSONDictionary = [[[NSMutableDictionary alloc] initWithCapacity:4] autorelease];
		
		NSMutableDictionary* touchViewsJSONDictionary = [[NSMutableDictionary alloc] initWithCapacity:4];
		[stateJSONDictionary setObject:touchViewsJSONDictionary forKey:
		 [NSString stringWithCString:JSON_TOUCH_VIEW_DICTIONARY_KEY encoding:NSUTF8StringEncoding]];
		[touchViewsJSONDictionary release];
		
		NSMutableDictionary* accelerationJSONDictionary = [[NSMutableDictionary alloc] initWithCapacity:3];
		[stateJSONDictionary setObject:accelerationJSONDictionary forKey:
		 [NSString stringWithCString:JSON_ACCELERATION_DICTIONARY_KEY encoding:NSUTF8StringEncoding]];
		[accelerationJSONDictionary release];
		
		self.jsonEncoder = [[SBJSON new] autorelease];
		
	}
	
	return self;
	
}

-(void) dealloc {
	
	self.touchRelayViews = nil;
	self.jsonEncoder = nil;
	self.stateJSONDictionary = nil;
	
	[super dealloc];
	
}

#pragma mark -
#pragma mark Sensor Packaging Methods


-(void) updateStateJSONDictionary {
	
	
	
	// UPDATE TOUCH JSON DESCRIPTION FOR ALL VIEWS IN THE TOUCHRELAYVIEWS DICTIONARY
	
	// Get and empty the json representation
	NSMutableDictionary* touchViewsJSONDictionary = [stateJSONDictionary objectForKey:
									  [NSString stringWithCString:JSON_TOUCH_VIEW_DICTIONARY_KEY 
														 encoding:NSUTF8StringEncoding]];
	[touchViewsJSONDictionary removeAllObjects];

	
	for (NSString* key in [touchRelayViews allKeys]) {
		
		NSMutableArray* touchJSONArray = [touchViewsJSONDictionary objectForKey:key];
		
		if (!touchJSONArray) {
			touchJSONArray = [[NSMutableArray alloc] initWithCapacity:4];
			[touchViewsJSONDictionary setObject:touchJSONArray forKey:key];
			[touchJSONArray release];
		}
		
		[touchJSONArray removeAllObjects];
		
		TouchRelayView* touchRelayView = [touchRelayViews objectForKey:key];
		
		for (UITouch* touch in touchRelayView.currentTouches) {
			[touchJSONArray addObject:[touch jsonPackageDictionary]];
		}
		
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

-(void) startRelaying {
	
	if (!relayTimer) {
		
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
	
	//NSLog(@"Sending current state");
	
	[self updateStateJSONDictionary];
	
	NSString* jsonString = [jsonEncoder stringWithObject:stateJSONDictionary];
	
	NSLog(@"Sending JSON State: %@", jsonString);
	
	NSData* jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
	
	[[ServerController instance] sendData:jsonData];
	
}

#pragma mark -
#pragma mark TouchRelayViews Methods

-(void) addTouchRelayView:(TouchRelayView*)relayView forKey:(NSString*)key {
	[touchRelayViews setObject:relayView forKey:key];
}

-(void) removeTouchRelayViewForKey:(NSString*)key {
	[touchRelayViews removeObjectForKey:key];
}

-(void) removeAllTouchRelayViews {
	[touchRelayViews removeAllObjects];
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
	[self stopRelaying];
	[self startRelaying];
	
}


@end
