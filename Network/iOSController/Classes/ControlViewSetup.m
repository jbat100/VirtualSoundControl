//
//  ControlViewSetup.m
//  iOSController
//
//  Created by Jonathan Thorpe on 13/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "ControlViewSetup.h"


@implementation ControlViewSetup


@synthesize normalisedFrame, descriptor, backgroundImageName, view;


-(id) init {
	
	if ((self = [super init])) {
		normalisedFrame = CGRectMake(0.0, 0.0, 1.0, 1.0);
	}
	
	return self;
	
}

-(void) dealloc {
	
	[descriptor release];
	[backgroundImageName release];
	[view release];
	
	[super dealloc];
	
}

-(void) encodeWithCoder:(NSCoder *)coder {
	
	[coder encodeCGRect:normalisedFrame forKey:@"ControlViewSetupNormalisedFrame"];
	[coder encodeObject:descriptor forKey:@"ControlViewSetupDescriptor"];
	[coder encodeObject:backgroundImageName forKey:@"ControlViewSetupBackgroundImageName"];

}

-(id) initWithCoder:(NSCoder *)coder {
	
	if ((self = [super init])) {
		
		self.normalisedFrame = [coder decodeCGRectForKey:@"ControlViewSetupNormalisedFrame"];
		self.descriptor = [coder decodeObjectForKey:@"ControlViewSetupDescriptor"];
		self.backgroundImageName = [coder decodeObjectForKey:@"ControlViewSetupBackgroundImageName"];
		
	}
	
	return self;
	
}

-(NSArray*) channels {
	
	return nil;
	
}
	 

@end
