//
//  SensorRelay.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
@class TouchRelayView;
@class SBJSON;




@interface SensorRelay : NSObject <UIAccelerometerDelegate> {
	
	@public
	
	NSTimeInterval updateInterval;
	
	@private
	
	NSMutableDictionary* touchRelayViews;
	UIAcceleration* currentAcceleration;
	NSMutableDictionary* stateJSONDictionary;
	SBJSON* jsonEncoder;
	NSTimer* relayTimer;
	
	
	
}

@property (assign) NSTimeInterval updateInterval;

+(SensorRelay*) instance;

-(void) addTouchRelayView:(TouchRelayView*)relayView forKey:(NSString*)key;
-(void) removeTouchRelayViewForKey:(NSString*)key;
-(void) removeAllTouchRelayViews;

-(void) startRelaying;
-(void) stopRelaying;

@end
