//
//  NSString+VSCAdditions.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 14/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import <string>

@interface NSString (VSCAdditions) 

+(NSString*) stringWithStdString:(std::string)s;

-(std::string) stdString;

-(NSSize) sizeWithFont:(NSFont*)font;


@end
