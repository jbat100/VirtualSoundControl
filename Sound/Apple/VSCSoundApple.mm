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


+(NSString*) labelForParameterWithKey:(VSCSParameter::Key)key {
	std::string label = VSCSParameter::getLabelForParameterWithKey(key);
	return [NSString stringWithCString:label.c_str() encoding:[NSString defaultCStringEncoding]];
}


+(id) tableColumnIdentifierForChannelIndex:(unsigned int)channelIndex {
	return [NSNumber numberWithUnsignedInt:channelIndex];
}

+(unsigned int) channelIndexForTableColumnIdentifier:(id)tableColumnIdentifier {
	return [tableColumnIdentifier unsignedIntValue];
}

@end

