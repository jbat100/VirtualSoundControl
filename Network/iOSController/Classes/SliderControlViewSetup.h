//
//  SliderControlViewSetup.h
//  iOSController
//
//  Created by Jonathan Thorpe on 16/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ControlViewSetup.h"

typedef enum _SliderOrientation {
	SliderOrientationHorizontal = 0,
	SliderOrientationVertical,
} SliderOrientation;

@protocol SliderControlViewSetup 

@property (nonatomic, retain) NSMutableArray* sliderSetups;
@property (nonatomic, assign) SliderOrientation sliderOrientation;

@end


@interface SliderControlViewSetup : ControlViewSetup {
	
	NSMutableArray* sliderSetups;
	SliderOrientation sliderOrientation;

}

@property (nonatomic, retain) NSMutableArray* sliderSetups;
@property (nonatomic, assign) SliderOrientation sliderOrientation;

-(void) setSliderSetup:(SliderSetup*)prototypeSetup forChannels:(NSArray*)channels;

@end
