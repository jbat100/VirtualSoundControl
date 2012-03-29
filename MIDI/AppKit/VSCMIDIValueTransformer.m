//
//  VSCMIDIValueTransformer.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/29/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCMIDIValueTransformer.h"

@implementation VSCMIDIValueTransformer

+ (Class)transformedValueClass { 
    return [NSNumber class]; 
}

+ (BOOL)allowsReverseTransformation { 
    return NO; 
}

- (id)transformedValue:(id)value {
    
    if (value == nil) {
        return [NSNumber numberWithInt:0];
    }
    
    return value;
}

@end
