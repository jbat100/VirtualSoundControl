//
//  VSCSoundInterfaceFactory.mm
//  SynthStation
//
//  Created by Jonathan Thorpe on 28/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundInterfaceFactory.h"


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

-(VSCSoundElementView*) interfaceViewForSoundElement:(VSCSoundElementPtr)soundElement {
    
    std::string elementType = soundElement->getValueForPropertyWithKey(VSCSProperty::KeySoundElementType);
	
	return nil;
	
}

@end
