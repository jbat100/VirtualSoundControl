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

@end
