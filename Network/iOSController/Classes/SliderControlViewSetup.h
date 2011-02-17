//
//  SliderControlViewSetup.h
//  iOSController
//
//  Created by Jonathan Thorpe on 16/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ControlViewSetup.h"

@protocol SliderControlViewSetup 

@property (nonatomic, retain) NSMutableArray* sliderRanges;
@property (nonatomic, retain) NSMutableArray* sliderValues;

@end


@interface SliderControlViewSetup : NSObject {
	
	NSMutableArray* sliderRanges;
	NSMutableArray* sliderValues;

}

@property (nonatomic, retain) NSMutableArray* sliderRanges;
@property (nonatomic, retain) NSMutableArray* sliderValues;

@end
