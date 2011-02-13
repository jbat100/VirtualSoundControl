//
//  SensorRelay.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ControlView.h"
@class SBJSON;

#define MINIMUM_UPDATE_INTERVAL 0.01


@interface ControlStation : NSObject <UIAccelerometerDelegate> {
	
	@public
	
	NSTimeInterval updateInterval;
	
	@private
	
	NSMutableArray* controlViews;
	UIAcceleration* currentAcceleration;
	NSMutableDictionary* stateJSONDictionary;
	SBJSON* jsonEncoder;
	NSTimer* relayTimer;
	
	
	
}

@property (assign) NSTimeInterval updateInterval;

+(ControlStation*) instance;

-(void) addControlView:(ControlView*)view;
-(void) removeControlView:(ControlView*)view;
-(void) removeAllTouchRelayViews;

-(void) startBroadcasting;
-(void) stopBroadcasting;

@end
