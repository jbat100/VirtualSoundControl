//
//  VSCSoundGeneratorControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 07/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundElementView.h"
#import "VSCParameterSliderControlView.h"
#import "VSCSoundApple.h"

#import "VSCSoundInterfaceFactory.h"

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

-(void) createDefaultInterface {
	
	VSCSParameter::KeyList keyList = soundElement->getInterfaceKeyList();
	
	VSCParameterSliderControlView* v = nil;
	v = [[VSCSoundInterfaceFactory defaultFactory] parameterSliderControlViewForParameterKeys:keyList];
	
	[v setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
	
	NSRect vBounds = v.bounds;
	NSRect selfBounds = self.bounds;
	
	selfBounds.size.height = vBounds.size.height;
	selfBounds.size.width = vBounds.size.width;

	self.bounds = selfBounds;
	
	[self addSubview:v];
}

-(void) setSoundElement:(VSCSoundElementPtr)element {
	soundElement = element;
}

-(VSCSoundElementPtr) getSoundElement {
	return soundElement;
}

#pragma mark VSCSoundParameterViewProtocol Methods

-(void) reloadParameterValues {
	// get parameter list from soundElement
}

-(double) doubleValueForParameterWithKey:(VSCSParameter::Key)key {
	
	for (id<VSCParameterControlViewProtocol> v in parameterControlViews) {

	}
	
	return 0.0;
}

-(void) reloadValueForParameterWithKey:(VSCSParameter::Key)key {
	
}


@end
