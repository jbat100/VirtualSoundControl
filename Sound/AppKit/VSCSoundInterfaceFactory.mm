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

-(VSCSoundElementView*) soundElementViewForSoundElement:(VSCSoundElementPtr)soundElement withFrame:(NSRect)f {
	
	VSCSoundElementView* v = [[VSCSoundElementView alloc] initWithFrame:f];
	
	[self configureSoundElementView:v ForSoundElement:soundElement];
	
	return [v autorelease];
	
}

-(void) configureSoundElementView:(VSCSoundElementView*)view ForSoundElement:(VSCSoundElementPtr)soundElement {
	
	std::string elementType = soundElement->getElementType();
	
	/*
	 *	Remove all parameter views
	 */
	NSArray* subviews = [view subviews];
	for (NSView* v in subviews) {
		if ([[view parameterControlViews] containsObject:v]) {
			[v removeFromSuperview];
		}
	}
	[[view parameterControlViews] removeAllObjects];
	
	/*
	 *
	 */
	 
	return;
	
	
}


-(VSCMatrixParameterControlView*) matrixParameterControlViewForParameterDomain:(VSCSParameter::Domain)domain
																	 withFrame:(NSRect)f
{
	VSCSParameter::KeySet keySet = VSCSParameter::sharedInstance().keysForDomain(domain);
	return [self matrixParameterControlViewForParameterKeys:keySet withFrame:f];	
}

-(VSCMatrixParameterControlView*) matrixParameterControlViewForParameterKeys:(VSCSParameter::KeySet)keys
																   withFrame:(NSRect)f 
{
	VSCMatrixParameterControlView* paramView = [[VSCMatrixParameterControlView alloc] initWithFrame:f];
	[paramView addParameterKeys:keys];
	[paramView createInterface];
	return [paramView autorelease];
	
}


@end
