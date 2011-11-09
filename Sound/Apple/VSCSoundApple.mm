//
//  VSCSoundApple.mm
//  SynthStation
//
//  Created by Jonathan Thorpe on 09/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundApple.h"
#import "VSCSynthSourceElement.h"

@implementation 


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

@end

