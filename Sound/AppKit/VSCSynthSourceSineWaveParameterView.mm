//
//  VSCSynthSourceSineWaveControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSynthSourceSineWaveParameterView.h"
#import "VSCSoundParameterView+Private.h"
#import "VSCSoundApple.h"
#import "VSCSoundParameters.h"

#import <boost/bimap.hpp>

typedef boost::bimap<VSCSParameter::Key, NSInteger> ParamKeyIndexMap;
typedef ParamKeyIndexMap::value_type ParamKeyIndexMapEntry;


@implementation VSCSynthSourceSineWaveParameterView

#pragma mark - NSView Methods

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

#pragma mark - Custom Initialization

-(void) customInit {
	
	[super customInit];
	
	paramKeyIndexMap.insert( ParamKeyIndexMapEntry(VSCSParameter::KeySineFrequency, 0) );
	paramKeyIndexMap.insert( ParamKeyIndexMapEntry(VSCSParameter::KeySinePhase, 1) );
	
	/*
	 *	Set sourceSine to NULL
	 */
	[self setSourceSine:VSCSynthSourceSinePtr()];
	
}


#pragma mark - C++ Setters/Getters


-(VSCSynthSourceSinePtr) getSourceSine {
	return sourceSine;
}

-(void) setSourceSine:(VSCSynthSourceSinePtr)_sourceSine {
	sourceSine = _sourceSine;
	[self resetInterface];
}


#pragma mark - VSCParameterControlViewDataSource Methods



-(NSString*) parameterControlView:(VSCParameterControlView*)view labelForParameterAtIndex:(NSInteger)index {
	VSCSParameter::Key key = [self keyForParameterAtIndex:index];
	return [VSCSoundApple labelForKey:key];
}

-(SEL) parameterControlView:(VSCParameterControlView*)view fetchSelectorForParameterAtIndex:(NSInteger)index {
	return @selector(parameterControlView:doubleForParameterAtIndex:);
}

-(double) parameterControlView:(VSCParameterControlView*)view doubleForParameterAtIndex:(NSInteger)index {
	
	VSCSParameter::Key key = [self keyForParameterAtIndex:index];
	
	if (!sourceSine) {
		return 0.0;
	}
	
	switch (key) {
			
		case VSCSParameter::KeySineFrequency :
			return sourceSine->getFrequency();
			break;
			
		case VSCSParameter::KeySinePhase :
			return sourceSine->getPhase();
			break;
			
		default:
			break;
	}
	
	return 0.0;
	
}


@end
