//
//  NSString+VSCAdditions.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 14/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "NSString+VSCAdditions.h"


@implementation NSString (VSCAdditions)

+(NSString*) stringWithStdString:(std::string)s {
	return [NSString stringWithCString:s.c_str() encoding:[NSString defaultCStringEncoding]];
}

-(std::string) stdString {
	std::string s([self UTF8String]);
	return s;
}

-(NSSize) sizeWithFont:(NSFont*)font {
	
	NSDictionary* attributes = [NSDictionary dictionaryWithObject:font forKey:NSFontAttributeName];
	
	return [self sizeWithAttributes:attributes];;
	
}

@end
