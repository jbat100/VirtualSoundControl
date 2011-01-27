//
//  UITouch+JSONPackaging.m
//  iOSController
//
//  Created by Jonathan Thorpe on 27/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "UITouch+JSONPackaging.h"

@implementation UITouch (JSONPackaging)

-(NSDictionary*) jsonPackageDictionary {
	
	NSMutableDictionary* dictionary = [[NSMutableDictionary alloc] initWithCapacity:6];
	
	CGPoint l = [self locationInView:[self view]];
	CGPoint pl = [self previousLocationInView:[self view]];
	
	[dictionary setObject:[NSString stringWithFormat:@"%x", self] forKey:@"id"];
	
	[dictionary setObject:[NSString stringWithFormat:@"%d", (NSInteger)(l.x)] forKey:@"x"];
	[dictionary setObject:[NSString stringWithFormat:@"%d", (NSInteger)(l.y)] forKey:@"y"];
	
	[dictionary setObject:[NSString stringWithFormat:@"%d", (NSInteger)(l.x - pl.x)] forKey:@"dx"];
	[dictionary setObject:[NSString stringWithFormat:@"%d", (NSInteger)(l.y - pl.y)] forKey:@"dy"];

	return dictionary;
	
}


@end
