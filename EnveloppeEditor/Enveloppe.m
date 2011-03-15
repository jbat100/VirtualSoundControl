//
//  Enveloppe.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 10/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "Enveloppe.h"


@implementation Enveloppe

@synthesize controlPoints;

-(id) init {
	if ((self = [super init])) {
		self.controlPoints = [NSMutableArray arrayWithCapacity:20];
	}
	return self;
}

-(void) dealloc {
	self.controlPoints = nil;
	[super dealloc];
}

-(id) initWithCoder:(NSCoder*)coder {
	
	if ((self = [super init])) {
		self.controlPoints = [coder decodeObjectForKey:@"EnveloppeControlPoints"];
	}
	
	return self;
}

-(void) encodeWithCoder:(NSCoder *)coder {
	
	[coder encodeObject:controlPoints forKey:@"EnveloppeControlPoints"];
	
}

@end
