//
//  SetupSchemaView.h
//  iOSController
//
//  Created by Jonathan Thorpe on 28/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ControlCenterSetup;


@interface SetupSchemaView : UIView {
	
	ControlCenterSetup* setup;
	
	NSInteger viewOutlineWidth;
	UIColor* viewOutlineColor;
	
	UIColor* touchViewBackgroundColor;
	UIColor* sliderViewBackgroundColor;
	UIColor* buttonViewBackgroundColor;
	
}

@property (nonatomic, assign) ControlCenterSetup* setup;

@property (nonatomic, assign) NSInteger viewOutlineWidth;
@property (nonatomic, retain) UIColor* viewOutlineColor;

@property (nonatomic, retain) UIColor* touchViewBackgroundColor;
@property (nonatomic, retain) UIColor* sliderViewBackgroundColor;
@property (nonatomic, retain) UIColor* buttonViewBackgroundColor;

@end
