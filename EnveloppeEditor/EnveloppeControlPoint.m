//
//  EnveloppeControlPoint.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 10/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "EnveloppeControlPoint.h"


@implementation EnveloppeControlPoint

@synthesize value, timeStamp;

+(EnveloppeControlPoint*) controlPointWithValue:(double)v andTimeStamp:(NSTimeInterval)t {
	return [[[EnveloppeControlPoint alloc] initWithValue:v andTimeStamp:t] autorelease];
}

-(id) initWithValue:(double)v andTimeStamp:(NSTimeInterval)t {
	if ((self = [super init])) {
		self.value = v;
		self.timeStamp = t;
	}
	return self;
}

-(id) initWithCoder:(NSCoder *)coder {
	
	if ((self = [super init])) {
		
		value = [coder decodeDoubleForKey:@"EnveloppeControlPointValue"];
		timeStamp = (NSTimeInterval)[coder decodeDoubleForKey:@"EnveloppeControlPointTimeStamp"];
		
	}
	
}

-(void) encodeWithCoder:(NSCoder *)coder {
	
	[coder encodeDouble:value forKey:@"EnveloppeControlPointValue"];
	[coder encodeDouble:(NSTimeInterval)timeStamp forKey:@"EnveloppeControlPointTimeStamp"];
	
}

@end
