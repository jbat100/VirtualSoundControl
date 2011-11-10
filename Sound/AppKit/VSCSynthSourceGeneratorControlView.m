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

#pragma mark Parameter index paths

+(NSDictionary*) parameterIndexPaths {
    return nil;
}


#pragma mark Adding/Removing Parameters

-(void) addParameterWithKey:(NSString*)key {
	

}

-(void) removeParameterWithKey:(NSString*)key {
	

}

-(void) removeAllParameters {
	

}


#pragma mark - VSCParameterControlViewDelegate Methods


-(void) parameterControlView:(VSCParameterControlView*)view changedParameterWithKey:(NSString*)key {
	
}

#pragma mark - VSCParameterControlViewDataSource Methods

/*
 *  These should never change
 */

-(NSInteger) parameterControlViewNumberOfParameters:(VSCParameterControlView *)view {
    return [[self parameterIndexPaths] count];
}


-(NSIndexPath*) parameterControlView:(VSCParameterControlView*)view indexPathForParameterWithKey:(NSString *)key  {
	return [[self parameterIndexPaths] objectForKey:key];
}

-(NSString *)parameterControlView:(VSCParameterControlView *)view parameterKeyForParameterAtIndexPath:(NSIndexPath *)indexPath {
    NSArray* keys = [[self parameterIndexPaths] keysOfEntriesWithOptions:0 passingTest:^BOOL(id key, id obj, BOOL *stop) {
        if ([indexPath isEqual:(NSIndexPath *)obj])
            return YES;
        return NO;
    }];
    if ([keys count] > 0)
        return [keys objectAtIndex:0];
    return nil;
}

-(SEL) parameterControlView:(VSCParameterControlView*)view fetchSelectorForParameterAtIndexPath:(NSIndexPath *)indexPath  {
    static SEL selector = @selector(parameterControlView:floatForParameterAtIndexPath:);
	return selector;
}

/*
 *  Return nil for all of these...
 */

-(NSString *)parameterControlView:(VSCParameterControlView *)view parameterKeyForParameterAtIndexPath:(NSIndexPath *)indexPath {
    return nil;
}


-(NSString*) parameterControlView:(VSCParameterControlView*)view displayStringForParameterAtIndexPath:(NSIndexPath *)indexPath {
	return nil;
}



@end
