//
//  Touch.m
//  VSoundStation
//
//  Created by Jonathan Thorpe on 03/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "Touch.h"


@implementation Touch

@synthesize touchId, xp, yp, dx, dy;

-(void) setWithJSONDict:(NSDictionary*)jsonDict {
	
	NSString* xString = [jsonDict objectForKey:JSON_TOUCH_X_KEY];
	NSString* yString = [jsonDict objectForKey:JSON_TOUCH_Y_KEY];
	NSString* dxString = [jsonDict objectForKey:JSON_TOUCH_DX_KEY];
	NSString* dyString = [jsonDict objectForKey:JSON_TOUCH_DY_KEY];
	
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
