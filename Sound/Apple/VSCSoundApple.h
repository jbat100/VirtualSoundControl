//
//  VSCSoundApple.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 09/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "VSCSoundParameters.h"


@interface VSCSoundApple : NSObject {
	
}

/*
 * No longer using string based parameter keys
 *
 
+(NSString*) VSCSoundFrequencyParameterKey;
+(NSString*) VSCSoundPhaseParameterKey;
+(NSString*) VSCSoundDBGainParameterKey;
+(NSString*) VSCSoundLinearGainParameterKey;
+(NSString*) VSCSoundHarmonicsParameterKey;
 
 
+(NSDictionary*) parameterLabels;
 
 */


+(NSString*) labelForKey:(VSCSParameter::Key)key;

@end
