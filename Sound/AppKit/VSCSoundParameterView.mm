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

typedef boost::bimap<VSCSParameter::Key, NSInteger> ParamKeyIndexBiMap;
typedef ParamKeyIndexBiMap::value_type ParamKeyIndexBiMapEntry;

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

-(void) reloadParameterLabels {
	NSInteger numberOfParameters = [self numberOfParameters];
	for (NSInteger i = 0; i < numberOfParameters; i++) {
		VSCSParameter::Key key = [self keyForParameterAtIndex:i];
		NSString* label = [self labelForParameterWithKey:key];
		[parameterControlView setLabel:label forParameterAtIndex:i];
	}
}

-(void) reloadParameterValues {
	NSInteger numberOfParameters = [self numberOfParameters];
	for (NSInteger i = 0; i < numberOfParameters; i++) {
		VSCSParameter::Key key = [self keyForParameterAtIndex:i];
		[self reloadValueForParameterWithKey:key];
	}
}

-(void) reloadParameterRanges {
	NSInteger numberOfParameters = [self numberOfParameters];
	for (NSInteger i = 0; i < numberOfParameters; i++) {
		VSCSParameter::Key key = [self keyForParameterAtIndex:i];
		NSString* label = [self labelForParameterWithKey:key];
		[parameterControlView setLabel:label forParameterAtIndex:i];
	}
}

-(void) reloadValueForParameterWithKey:(VSCSParameter::Key)key {
    NSInteger i = [self indexForParameterWithKey:key];
	double val = [self doubleValueForParameterWithKey:key];
	[parameterControlView setDoubleValue:val forParameterAtIndex:i];
}

#pragma mark VSCSoundParameterViewProtocol Methods

-(VSCSParameter::Key) keyForParameterAtIndex:(NSInteger)index {
	
	ParamKeyIndexBiMap::right_const_iterator right_iter = paramKeyIndexMap.right.find(index);
	
	// couldn't find the index in the bimap...
	if (right_iter == paramKeyIndexMap.right.end()) 
		return VSCSParameter::KeyNone;
	
	return right_iter->second;	
}

-(NSInteger) indexForParameterWithKey:(VSCSParameter::Key)key {
	
	ParamKeyIndexBiMap::left_const_iterator left_iter = paramKeyIndexMap.left.find(key);
	
	// couldn't find the key in the bimap...
	if (left_iter == paramKeyIndexMap.left.end()) 
		return -1;
	
	return left_iter->second;
	
}

-(NSInteger) numberOfParameters {
	return paramKeyIndexMap.size();
}

-(NSString*) labelForParameterWithKey:(VSCSParameter::Key)key {
	return [VSCSoundApple labelForKey:key];
}

-(double) doubleValueForParameterWithKey:(VSCSParameter::Key)key {
	
	NSInteger index = [self indexForParameterWithKey:key];
	
	NSAssert();
	
	return 0.0;
}

-(std::pair<double, double>) rangeForParameterWithKey:(VSCSParameter::Key)key {
    return VSCSParameter::getRangeForParameterWithKey(key);
}

#pragma mark - VSCParameterControlViewDelegate Methods

-(void) parameterControlView:(id<VSCParameterControlViewProtocol>)view 
	 changedParameterAtIndex:(NSUInteger)index {
	double val = [view getParameterAtIndex:index];
	NSLog(@"%@ received parameter change from %@, value is %f", self, view, val);
}



@end
