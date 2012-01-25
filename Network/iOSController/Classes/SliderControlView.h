//
//  SliderControlView.h
//  iOSController
//
//  Created by Jonathan Thorpe on 09/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ControlView.h"

@interface SliderControlView : ControlView {
	
	NSMutableArray* sliders;
	NSMutableArray* changedSliders;

}

-(void) updateSliderSetups;
-(void) sliderValueChanged:(id)sender;
-(void) clearChangedSliders;



@end
