//
//  Touch.m
//  VSoundStation
//
//  Created by Jonathan Thorpe on 03/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "Touch.h"
#import "NetworkDefinitions.h"


@implementation Touch

@synthesize touchId, xp, yp, dx, dy, displayColour, colourIndex;

-(id) init {
	
	if ((self = [super init])) {
		colourIndex = -1;
	}
	
	return self;
	
}

-(NSString*) description {
	
	return [NSString stringWithFormat:@"%@, with id: %@, x=%.4f, y=%.4f, dx=%.4f, dy=%.4f", 
			[super description], touchId, xp, yp, dx, dy];
	
}

-(BOOL) isEqualToTouch:(Touch*)t {
	
	return [touchId isEqualToString:t.touchId];
	
}

-(BOOL) isEqual:(id)object {
	
	if (![object isKindOfClass:[Touch class]]) 
		return NO;
	
	return [self isEqualToTouch:object];
	
}

-(void) setWithJSONDict:(NSDictionary*)jsonDict {
	
	self.touchId = [jsonDict objectForKey:[NSString stringWithUTF8String:JSON_TOUCH_ID_KEY]];
	
	NSString* xString = [jsonDict objectForKey:[NSString stringWithUTF8String:JSON_TOUCH_X_KEY]];
	NSString* yString = [jsonDict objectForKey:[NSString stringWithUTF8String:JSON_TOUCH_Y_KEY]];
	NSString* dxString = [jsonDict objectForKey:[NSString stringWithUTF8String:JSON_TOUCH_DX_KEY]];
	NSString* dyString = [jsonDict objectForKey:[NSString stringWithUTF8String:JSON_TOUCH_DY_KEY]];
	
	NSScanner* scanner;
	
	scanner = [NSScanner scannerWithString:xString];
	[scanner scanFloat:&xp];
	
	scanner = [NSScanner scannerWithString:yString];
	[scanner scanFloat:&yp];
	
	scanner = [NSScanner scannerWithString:dxString];
	[scanner scanFloat:&dx];
	
	scanner = [NSScanner scannerWithString:dyString];
	[scanner scanFloat:&dy];
	
}

@end
