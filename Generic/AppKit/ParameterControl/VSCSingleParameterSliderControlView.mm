//
//  VSCSingleParameterSliderControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 10/12/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSingleParameterSliderControlView.h"


@implementation VSCSingleParameterSliderControlView

@synthesize labelTextField;
@synthesize minTextField;
@synthesize maxTextField;
@synthesize numericTextField;
@synthesize controlSlider;

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

-(void) updateInterface {
	[self.controlSlider setDoubleValue:self.doubleValue];
	[self.controlSlider setMinValue:self.valueRange.first];
	[self.controlSlider setMaxValue:self.valueRange.second];
	[self.minTextField setStringValue:[NSString stringWithFormat:@"%f.2", self.valueRange.first]];
	[self.maxTextField setStringValue:[NSString stringWithFormat:@"%f.2", self.valueRange.second]];
	[self.labelTextField setStringValue:label];
}

-(IBAction) controlSliderChanged:(id)sender {
	NSSlider* slider = (NSSlider*)sender;
	self.doubleValue = [slider doubleValue];
	[listener object:self changedParameterWithKey:self.key to:doubleValue];
}

-(void) setDoubleValue:(double)val {
	self.doubleValue = val;
	[self.controlSlider setDoubleValue:val];
}

-(void) setValueRange:(VSCSParameter::ValueRange)valRange {
	self.valueRange = valRange;
	[self.controlSlider setMinValue:self.valueRange.first];
	[self.controlSlider setMaxValue:self.valueRange.second];
	[self.minTextField setStringValue:[NSString stringWithFormat:@"%f.2", valRange.first]];
	[self.maxTextField setStringValue:[NSString stringWithFormat:@"%f.2", valRange.second]];
}

-(void) setLabel:(NSString *)lab {
	[label release];
	label = [lab copy];
	[self.labelTextField setStringValue:label];
}


@end
