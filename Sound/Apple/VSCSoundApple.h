//
//  VSCSoundApple.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 09/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface VSCSoundApple : NSObject {
	
}

+(NSString*) VSCSoundFrequencyParameterKey;
+(NSString*) VSCSoundPhaseParameterKey;
+(NSString*) VSCSoundDBGainParameterKey;
+(NSString*) VSCSoundLinearGainParameterKey;
+(NSString*) VSCSoundHarmonicsParameterKey;

+(NSDictionary*) parameterDisplayStrings;
+(NSString*) displayStringForKey:(NSString*)key;

@end
