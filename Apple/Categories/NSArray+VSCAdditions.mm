//
//  NSString+VSCAdditions.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 14/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "NSArray+VSCAdditions.h"


@implementation NSArray (VSCAdditions)

- (id)firstObject
{
    if ([self count] > 0)
    {
        return [self objectAtIndex:0];
    }
    return nil;
}

@end
