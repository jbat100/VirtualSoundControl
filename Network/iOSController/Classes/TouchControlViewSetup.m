//
//  TouchControlViewSetup.m
//  iOSController
//
//  Created by Jonathan Thorpe on 13/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "TouchControlViewSetup.h"


@implementation TouchControlViewSetup

@synthesize xChannel, yChannel;

-(id) init {
	
	if ((self = [super init])) {
		xChannel = 1;
		yChannel = 2;
	}
	
	return self;
	
}

-(void) encodeWithCoder:(NSCoder *)coder {
	
	[super encodeWithCoder:coder];
	
	[coder encodeInteger:xChannel forKey:@"TouchControlViewSetupXChannel"];
	[coder encodeInteger:yChannel forKey:@"TouchControlViewSetupYChannel"];
	
}

-(id) initWithCoder:(NSCoder *)coder {
	
	if ((self = [super initWithCoder:coder])) {
		
		self.xChannel = [coder decodeCGRectForKey:@"TouchControlViewSetupXChannel"];
		self.yChannel = [coder decodeObjectForKey:@"TouchControlViewSetupYChannel"];
		
	}
	
	return self;
	
}

-(NSArray*) channels {
	
	return [NSArray arrayWithObjects:[NSNumber numberWithInteger:xChannel], [NSNumber numberWithInteger:yChannel], nil];
	
}

@end
