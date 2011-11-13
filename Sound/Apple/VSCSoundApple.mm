//
//  VSCSoundApple.mm
//  SynthStation
//
//  Created by Jonathan Thorpe on 09/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundApple.h"
#import "VSCSynthSourceElement.h"

@implementation VSCSoundApple


+(NSString*) labelForKey:(VSCSParameter::Key)key {
	
	std::string label = VSCSParameter::labelForParameterWithKey(key);
	
	return [NSString stringWithCString:label.c_str() encoding:[NSString defaultCStringEncoding]];
	
}

/*
 *
 *	Abandonned string based praameter keys in favour of enum for faster comparisons
 *

static NSDictionary* parameterDisplayStrings = nil;

#pragma mark Parameter Keys

+(NSString*) VSCSoundFrequencyParameterKey {
	
	static NSString* vscSoundFrequencyParameterKey =  
	[NSString stringWithCString:kVSCSoundFrequencyParameterKey.c_str() encoding:[NSString defaultCStringEncoding]];

	return vscSoundFrequencyParameterKey;
	
}

+(NSString*) VSCSoundPhaseParameterKey {
	
	static NSString* vscSoundPhaseParameterKey =  
	[NSString stringWithCString:kVSCSoundPhaseParameterKey.c_str() encoding:[NSString defaultCStringEncoding]];
	
	return vscSoundPhaseParameterKey;
	
}

+(NSString*) VSCSoundDBGainParameterKey {
	
	static NSString* vscSoundDBGainParameterKey =  
	[NSString stringWithCString:kVSCSoundDBGainParameterKey.c_str() encoding:[NSString defaultCStringEncoding]];
	
	return vscSoundDBGainParameterKey;
	
}

+(NSString*) VSCSoundLinearGainParameterKey {
	
	static NSString* vscSoundLinearGainParameterKey =  
	[NSString stringWithCString:kVSCSoundLinearGainParameterKey.c_str() encoding:[NSString defaultCStringEncoding]];
	
	return vscSoundLinearGainParameterKey;
	
}

+(NSString*) VSCSoundHarmonicsParameterKey {
	
	static NSString* vscSoundHarmonicsParameterKey =  
	[NSString stringWithCString:kVSCSoundHarmonicsParameterKey.c_str() encoding:[NSString defaultCStringEncoding]];
	
	return vscSoundHarmonicsParameterKey;
	
}

#pragma mark Parameter Display Strings

+(NSDictionary*) parameterDisplayStrings {
    
    if (parameterDisplayStrings == nil) {
        parameterDisplayStrings = [NSDictionary dictionaryWithObjectsAndKeys:
                                   @"Frequency (Hz)", [VSCSoundApple VSCSoundFrequencyParameterKey], 
                                   @"Phase (Degrees)", [VSCSoundApple VSCSoundPhaseParameterKey],
                                   @"Gain (dB)", [VSCSoundApple VSCSoundDBGainParameterKey],
                                   @"Gain (Linear)", [VSCSoundApple VSCSoundDBGainParameterKey],
                                   @"Number of Harmonics", [VSCSoundApple VSCSoundHarmonicsParameterKey],
                                   nil];
    }
    
    return parameterDisplayStrings;
    
}

+(NSString*) displayStringForKey:(NSString*)key {
    
    return [[VSCSoundApple parameterDisplayStrings] objectForKey:key];
    
}
 
 */

@end

