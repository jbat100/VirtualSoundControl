//
//  VSCSoundInterfaceFactory.mm
//  SynthStation
//
//  Created by Jonathan Thorpe on 28/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundInterfaceFactory.h"

#import "VSCSoundMultiChannelElement.h"
#import "VSCSoundSine.h"

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
	
	
	VSCSoundMultiChannelElementPtr multiChannelPtr = 
	boost::dynamic_pointer_cast<VSCSoundMultiChannelElement> (soundElement);
	
	if (multiChannelPtr) {
		VSCSParameter::KeySet parameterKeys;
		VSCSParameter::KeyIndexBimap parameterIndexBimap;
		unsigned int numberOfChannels = multiChannelPtr->getNumberOfChannels();
		for (unsigned int i = 0; i < numberOfChannels; i++) {
			VSCSParameter::Key key = {VSCSParameter::DomainChannel, VSCSParameter::CodeDBGain, i};
			parameterKeys.insert(key);
			VSCSParameter::KeyIndexBimapEntry entry (i,i);
			parameterIndexBimap.insert(entry);
		}
		NSRect f = view.frame;
		VSCMatrixParameterControlView* controlView = [self matrixParameterControlViewForParameterKeys:parameterKeys 
																					withKeyIndexBimap:parameterIndexBimap 
																							withFrame:f];

	}
	 
	return;
	
	
}




-(VSCMatrixParameterControlView*) matrixParameterControlViewForParameterKeys:(VSCSParameter::KeySet)keys
														   withKeyIndexBimap:(VSCSParameter::KeyIndexBimap)map
																   withFrame:(NSRect)f 
{
	VSCMatrixParameterControlView* matrixControlView = [[VSCMatrixParameterControlView alloc] initWithFrame:f];
	[matrixControlView addParameterKeys:parameterKeys];
	[matrixControlView setparameterKeyIndexBimap:parameterIndexBimap];
	[matrixControlView createInterface];
	return [matrixControlView autorelease];
	
}


@end
