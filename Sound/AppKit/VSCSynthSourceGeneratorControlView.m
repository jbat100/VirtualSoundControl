//
//  VSCSynthSourceGeneratorControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 07/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSynthSourceGeneratorControlView.h"


@implementation VSCSynthSourceGeneratorControlView

@synthesize parameterKeys, parameterControlSlidersView;

- (id)initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
    if (self) {
        // Initialization code here.
    }
    return self;
}

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

-(void) customInit {
	self.parameterKeys = [NSMutableSet setWithCapacity:10];
}


#pragma mark Adding/Removing Parameters

-(void) addParameterWithKey:(NSString*)key {
	
	[parameterKeys addObject:key];
	[parameterControlSlidersView updateInterface];
	
}

-(void) removeParameterWithKey:(NSString*)key {
	
	[parameterKeys removeObject:key];
	
	[parameterControlSlidersView updateInterface];
	
}

-(void) removeAllParameters {
	
	[parameterKeys removeAllObjects];
	
	[parameterControlSlidersView updateInterface];
	
}


#pragma mark - VSCParameterControlViewDelegate Methods


-(void) parameterControlView:(VSCParameterControlView*)view changedParameterWithKey:(NSString*)key {
	
}

#pragma mark - VSCParameterControlViewDataSource Methods


-(NSMutableSet*) parameterControlViewParameterKeys:(VSCParameterControlView*)view {
	return [[parameterKeys retain] autorelease];
}

-(NSString*) parameterControlView:(VSCParameterControlView*)view displayStringForParameterWithKey:(NSString*)key {
	return nil;
}

-(NSInteger) parameterControlView:(VSCParameterControlView*)view displayIndexForParameterWithKey:(NSString*)key {
	return nil;
}

-(SEL) parameterControlView:(VSCParameterControlView*)view fetchSelectorForParameterWithKey:(NSString*)key {
	return nil;
}

-(NSString*) parameterControlView:(VSCParameterControlView*)view stringForParameterWithKey:(NSString*)key {
	return nil;
}

-(float) parameterControlView:(VSCParameterControlView*)view floatForParameterWithKey:(NSString*)key {
	return 0.0;
}

-(double) parameterControlView:(VSCParameterControlView*)view doubleForParameterWithKey:(NSString*)key {
	return 0.0;
}

@end
