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
	
	TouchRelayView* touchRelayView;
	UIAcceleration* currentAcceleration;
	
	NSMutableArray* touchJSONArray;
	NSMutableDictionary* accelerationJSONDictionary;
	NSMutableDictionary* stateJSONDictionary;
	
	SBJSON* jsonEncoder;
	NSTimer* relayTimer;
	
}

@property (nonatomic, retain) TouchRelayView* touchRelayView;
@property (nonatomic, retain) UIAcceleration* currentAcceleration;

+(SensorRelay*) instance;

-(void) sendCurrentState;
-(void) updateTouchJSONArray;

@end
