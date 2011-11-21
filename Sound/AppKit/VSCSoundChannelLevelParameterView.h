//
//  VSCSoundChannelLevelControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 12/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCSoundParameterView.h"
#import "VSCSoundMultiChannelElement.h"


@interface VSCSoundChannelLevelParameterView : VSCSoundParameterView {
	
	VSCSoundMultiChannelElementPtr multiChannelElement;
	
	/*
	 *	Keep track of the current number of channels 
	 */
	NSInteger currentNumberOfChannels;

}

-(VSCSoundMultiChannelElementPtr) getMultiChannelElement;
-(void) setMultiChannelElement:(VSCSoundMultiChannelElementPtr)_multiChannelElement;


@end
