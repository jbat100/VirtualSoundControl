//
//  VSCSoundGeneratorControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 07/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundElementView.h"
#import "VSCParameterControlView.h"
#import "VSCMatrixParameterControlView.h"
#import "VSCSoundApple.h"



@implementation VSCSoundElementView

@synthesize parameterControlViews;

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
	
}



-(void) reloadValueForParameterWithKey:(VSCSParameter::Key)key {

}

-(void) setSoundElement:(VSCSoundElementPtr)element {
	soundElement = element;
}

-(VSCSoundElementPtr) getSoundElement {
	return soundElement;
}

#pragma mark VSCSoundParameterViewProtocol Methods

-(double) doubleValueForParameterWithKey:(VSCSParameter::Key)key {
	return 0.0;
}



#pragma mark - VSCParameterControlViewDelegate Methods

-(void) parameterControlView:(id<VSCParameterControlViewProtocol>)view 
	 changedParameterWithKey:(VSCSParameter::Key)key
						  to:(double)val {
	NSLog(@"%@ received parameter change from %@, value is %f", self, view, val);
}

-(BOOL) interestedInParameterId:(VSCSParameterId)paramId {
	if (paramId.element == sourceGenerator.get()) {
		return YES;
	}
	return NO;
}


@end
