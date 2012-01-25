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
	[controlSlider setDoubleValue:self.doubleValue];
	[controlSlider setMinValue:self.valueRange.first];
	[controlSlider setMaxValue:self.valueRange.second];
	[minTextField setStringValue:[NSString stringWithFormat:@"%f.2", self.valueRange.first]];
	[maxTextField setStringValue:[NSString stringWithFormat:@"%f.2", self.valueRange.second]];
	[labelTextField setStringValue:label];
}

-(IBAction) controlSliderChanged:(id)sender {
	NSSlider* slider = (NSSlider*)sender;
	self.doubleValue = [slider doubleValue];
	[listener object:self changedParameterWithKey:self.key to:doubleValue];
}

-(void) setDoubleValue:(double)val {
	doubleValue = val;
	[self.controlSlider setDoubleValue:val];
    [numericTextField setStringValue:[NSString stringWithFormat:@"%.2f", val]];
}

-(void) setValueRange:(VSCSParameter::ValueRange)valRange {
	valueRange = valRange;
	[controlSlider setMinValue:self.valueRange.first];
	[controlSlider setMaxValue:self.valueRange.second];
	[minTextField setStringValue:[NSString stringWithFormat:@"%.2f", valRange.first]];
	[maxTextField setStringValue:[NSString stringWithFormat:@"%.2f", valRange.second]];
}

-(void) setLabel:(NSString *)lab {
	[label release];
	label = [lab copy];
	[self.labelTextField setStringValue:label];
}


@end
