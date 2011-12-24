//
//  VSCSoundInterfaceFactory.mm
//  SynthStation
//
//  Created by Jonathan Thorpe on 28/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundInterfaceFactory.h"

#import "VSCSoundElement.h"
#import "VSCSoundSine.h"
#import "VSCParameterSliderControlView.h"

#import <boost/pointer_cast.hpp>


@implementation VSCSoundInterfaceFactory

@synthesize theme, options;

static VSCSoundInterfaceFactory* defaultFactoryInstance = nil;

+(VSCSoundInterfaceFactory*) defaultFactory {
	
	if (!defaultFactoryInstance) {
		@synchronized (self) {
			defaultFactoryInstance = [[VSCSoundInterfaceFactory alloc] init];
		}
	}
	
	return defaultFactoryInstance;
	
}

-(id) init {
	
	if ((self = [super init])) {
		self.theme = VSCSoundInterfaceFactoryThemeDefault;
		self.options = VSCSoundInterfaceFactoryOptionNone;
	}
	
    return self;
}

-(VSCParameterSliderControlView*) parameterSliderControlViewForParameterKeys:(VSCSParameter::KeyList)keys;
{
	NSRect f = NSZeroRect;
	VSCParameterSliderControlView* controlView = [[VSCParameterSliderControlView alloc] initWithFrame:f];
	[controlView addParameterKeys:keys];
	[controlView createInterface];
	return [controlView autorelease];
}


@end
