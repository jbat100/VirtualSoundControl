//
//  VSCSingleParameterSliderControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 10/12/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCSingleParameterControlView.h"

@interface VSCSingleParameterSliderControlView : VSCSingleParameterControlView {
	
	NSTextField* labelTextField;
	NSTextField* minTextField;
	NSTextField* maxTextField;
	NSTextField* numericTextField;
	NSSlider* controlSlider;

}

@property (nonatomic, retain) IBOutlet NSTextField* labelTextField;
@property (nonatomic, retain) IBOutlet NSTextField* minTextField;
@property (nonatomic, retain) IBOutlet NSTextField* maxTextField;
@property (nonatomic, retain) IBOutlet NSTextField* numericTextField;
@property (nonatomic, retain) IBOutlet NSSlider* controlSlider;

-(IBAction) controlSliderChanged:(id)sender;

@end
