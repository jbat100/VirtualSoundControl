//
//  VSCSynthSourceGeneratorControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 07/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundParameterView.h"
#import "VSCSoundParameterView+Private.h"
#import "VSCParameterControlView.h"
#import "VSCSoundApple.h"



@implementation VSCSoundParameterView

@synthesize parameterControlView;

- (id)initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
    if (self) {
        // Initialization code here.
		[self customInit];
    }
    return self;
}

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		[self customInit];
    }
    return self;
}

/*
- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}
 */

-(void) customInit {
	[self createParameterControlView];
}

-(void) createParameterControlView {
	self.parameterControlView = [[[VSCParameterControlView alloc] initWithFrame:self.bounds] autorelease];
	[self addSubview:parameterControlView];
	parameterControlView.delegate = self;
}

-(void) createParameterControlInterface {
	[parameterControlView createInterfaceForParameterCount:[self numberOfParameters]];
}

-(void) updateParameterKeyIndexMap {
	
}



-(void) reloadValueForParameterWithKey:(VSCSParameter::Key)key {
    NSInteger i = [self indexForParameterWithKey:key];
	double val = [self doubleValueForParameterWithKey:key];
	[parameterControlView setDoubleValue:val forParameterAtIndex:i];
}

#pragma mark VSCSoundParameterViewProtocol Methods

-(double) doubleValueForParameterWithKey:(VSCSParameter::Key)key {
	return 0.0;
}

-(std::pair<double, double>) rangeForParameterWithKey:(VSCSParameter::Key)key {
    return VSCSParameter::getRangeForParameterWithKey(key);
}

#pragma mark - VSCParameterControlViewDelegate Methods

-(void) parameterControlView:(id<VSCParameterControlViewProtocol>)view 
	 changedParameterAtIndex:(NSUInteger)index {
	double val = [view getDoubleValueForParameterAtIndex:index];
	NSLog(@"%@ received parameter change from %@, value is %f", self, view, val);
}



@end
