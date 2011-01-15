//
//  SensorRelay.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>



@interface SensorRelay : NSObject <UIAccelerometerDelegate> {
	
	UIAcceleration* currentAcceleration;
	
	
}

@property (nonatomic, retain) UIAcceleration* currentAcceleration;

+(SensorRelay*) instance;

-(void) sendCurrentState;

@end
