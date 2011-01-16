//
//  SensorRelay.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
@class TouchRelayView;



@interface SensorRelay : NSObject <UIAccelerometerDelegate> {
	
	TouchRelayView* touchRelayView
	UIAcceleration* currentAcceleration;
	
	NSTimer* relayTimer;
	
	
}

@property (nonatomic, retain) UIAcceleration* currentAcceleration;

+(SensorRelay*) instance;

-(void) sendCurrentState;

@end
