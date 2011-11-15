//
//  NSString+VSCAdditions.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 14/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "NSString+VSCAdditions.h"


@implementation NSString (VSCAdditions)

-(NSSize) sizeWithFont:(NSFont*)font {
	
	NSFont* font = [cell font];
	NSDictionary* attributes = [NSDictionary dictionaryWithObject:font forKey:NSFontAttributeName];
	
	return [s sizeWithAttributes:attributes];;
	
}

@end
