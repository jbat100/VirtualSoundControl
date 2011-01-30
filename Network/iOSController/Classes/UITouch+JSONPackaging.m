//
//  UITouch+JSONPackaging.m
//  iOSController
//
//  Created by Jonathan Thorpe on 27/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "UITouch+JSONPackaging.h"
#import "NetworkDefinitions.h"

@implementation UITouch (JSONPackaging)

-(NSDictionary*) jsonPackageDictionary {
	
	NSMutableDictionary* dictionary = [[NSMutableDictionary alloc] initWithCapacity:6];
	
	CGPoint l = [self locationInView:[self view]];
	CGPoint pl = [self previousLocationInView:[self view]];
	
	[dictionary setObject:[NSString stringWithFormat:@"%x", self] forKey:
	 [NSString stringWithCString:JSON_TOUCH_ID_KEY encoding:NSUTF8StringEncoding]];
	
	[dictionary setObject:[NSString stringWithFormat:@"%d", (NSInteger)(l.x)] forKey:
	 [NSString stringWithCString:JSON_TOUCH_X_KEY encoding:NSUTF8StringEncoding]];
	[dictionary setObject:[NSString stringWithFormat:@"%d", (NSInteger)(l.y)] forKey:
	 [NSString stringWithCString:JSON_TOUCH_Y_KEY encoding:NSUTF8StringEncoding]];
	
	[dictionary setObject:[NSString stringWithFormat:@"%d", (NSInteger)(l.x - pl.x)] forKey:
	 [NSString stringWithCString:JSON_TOUCH_DX_KEY encoding:NSUTF8StringEncoding]];
	[dictionary setObject:[NSString stringWithFormat:@"%d", (NSInteger)(l.y - pl.y)] forKey:
	 [NSString stringWithCString:JSON_TOUCH_DY_KEY encoding:NSUTF8StringEncoding]];

	return [NSDictionary dictionaryWithDictionary:dictionary];
	
}


@end
