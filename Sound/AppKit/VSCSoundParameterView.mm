//
//  VSCSynthSourceGeneratorControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 07/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundParameterView.h"
#import "VSCParameterControlSlidersView.h"

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
	
	self.parameterControlView = [[[VSCParameterControlSlidersView alloc] initWithFrame:self.bounds] autorelease];
	[self addSubview:parameterControlView];
	parameterControlView.delegate = self;
	
}

#pragma mark VSCSoundParameterViewProtocol Methods

-(VSCSParameter::Key) keyForParameterAtIndex:(NSInteger)index {
	
	ParamKeyIndexBiMap::right_const_iterator right_iter = paramKeyIndexMap.right.find(index);
	
	// couldn't find the index in the bimap...
	if (right_iter == paramKeyIndexMap.right.end()) 
		VSCSParameter::KeyNone;
	
	return right_iter->second;	
}

-(NSInteger) indexForParameterWithKey:(VSCSParameter::Key)key {
	
	ParamKeyIndexBiMap::left_const_iterator left_iter = paramKeyIndexMap.left.find(key);
	
	// couldn't find the key in the bimap...
	if (left_iter == paramKeyIndexMap.left.end()) 
		return -1;
	
	return left_iter->second;
	
}

#pragma mark - VSCParameterControlViewDelegate Methods


-(void) parameterControlView:(VSCParameterControlView*)view changedParameterWithKey:(VSCSParameter::Key)key {
	
}

#pragma mark - VSCParameterControlViewDataSource Methods

-(NSInteger) parameterControlViewNumberOfParameters:(VSCParameterControlView*)view {
	return paramKeyIndexMap.size();
}

-(NSInteger) parameterControlView:(VSCParameterControlView*)view indexForParameterWithKey:(VSCSParameter::Key)key {
	return [self indexForParameterWithKey:key];
}

-(VSCSParameter::Key) parameterControlView:(VSCParameterControlView*)view keyForParameterAtIndex:(NSInteger)index {
    return [self keyForParameterAtIndex:index];	
}

-(NSString*) parameterControlView:(VSCParameterControlView*)view labelForParameterAtIndex:(NSInteger)index {
	return nil;
}

-(SEL) parameterControlView:(VSCParameterControlView*)view fetchSelectorForParameterAtIndex:(NSInteger)index {
	return nil;
}





@end
