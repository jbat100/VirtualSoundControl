//
//  SliderSetup.h
//  iOSController
//
//  Created by Jonathan Thorpe on 22/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface SliderSetup : NSObject <NSCoding> {
	
	float value;
	
	float minValue;
	float maxValue;
	
	NSInteger channel;

}

@property (nonatomic, assign) float value;

@property (nonatomic, assign) float minValue;
@property (nonatomic, assign) float maxValue;

@property (nonatomic, assign) NSInteger channel;

@end
