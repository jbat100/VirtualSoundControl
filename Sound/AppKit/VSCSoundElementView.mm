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
#import "VSCSoundParameters.h"
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
    
    /*
     *  Clear previous interface
     */
    NSArray* subviews = [NSArray arrayWithArray:[self subviews]];
    for (NSView* v in subviews) {
        if ([v isKindOfClass:[VSCParameterSliderControlView class]]) [v removeFromSuperview];
    }
	VSCSParameter::KeyList keyList = soundElement->getInterfaceKeyList();
    
    NSLog(@"Creating default interface, Interface key list (%lu elements):", keyList.size());
    for (VSCSParameter::KeyList::iterator keyIt = keyList.begin(); keyIt != keyList.end(); keyIt++) {
        if (keyIt != keyList.begin()) std::cout << ", ";
        std::cout << VSCSParameter::sharedInstance().getLabelForParameterWithKey(*keyIt);
    }
    std::cout << std::endl;
    
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
    [self createDefaultInterface];
    [self reloadParameterValues];
}

-(VSCSoundElementPtr) getSoundElement {
	return soundElement;
}

#pragma mark VSCSoundParameterViewProtocol Methods

-(void) reloadParameterValues {
    VSCSParameter::KeyList keyList = soundElement->getInterfaceKeyList();
    for (VSCSParameter::KeyList::iterator keyIt = keyList.begin(); keyIt != keyList.end(); keyIt++) {
        [self reloadValueForParameterWithKey:(*keyIt)];
    }
}

-(double) doubleValueForParameterWithKey:(VSCSParameter::Key)key {
	VSCSFloat val = soundElement->getValueForParameterWithKey(key);
	return val;
}

-(void) reloadValueForParameterWithKey:(VSCSParameter::Key)key {
    
    NSLog(@"Reloading value for for parameter with key:");
    std::cout << VSCSParameter::sharedInstance().getLabelForParameterWithKey(key);
    double val = [self doubleValueForParameterWithKey:key];
    for (id<VSCParameterControlViewProtocol> v in self.parameterControlViews) {
        [v setDoubleValue:val forParameterWithKey:key];
    }
	
}


@end

