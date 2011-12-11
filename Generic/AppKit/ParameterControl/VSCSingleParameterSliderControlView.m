//
//  VSCSingleParameterSliderControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 10/12/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSingleParameterSliderControlView.h"


@implementation VSCSingleParameterSliderControlView

@synthesize labelTextField = _labelTextField;
@synthesize minTextField = _minTextField;
@synthesize maxTextField = _maxTextField;
@synthesize numericTextField = _numericTextField;
@synthesize controlSlider = _controlSlider;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

-(IBAction) controlSliderChanged:(id)sender {
	NSSlider* slider = (NSSlider*)sender;
	doubleValue = [slider doubleValue];
	[listener object:self changedParameterWithKey:self.key to:doubleValue];
}

-(void) setDoubleValue:(double)val {
	doubleValue = val;
	[self.controlSlider setDoubleValue:val];
}

-(void) setValueRange:(VSCSParameter::ValueRange)valRange {
	valueRange = valRange;
	[self.controlSlider setMinValue:valRange.first];
	[self.controlSlider setMaxValue:valRange.second];
}

-(void) setLabel:(NSString *)lab {
	[label release];
	label = [lab copy];
	[self.labelTextField setStringValue:label];
}


@end
