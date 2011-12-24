//
//  VSCParameterControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 27/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCParameterControlViewProtocol.h"
#import "VSCParameterListenerProtocol.h"
#import "VSCSoundParameters.h"

#import <map>
#import <list>

@interface VSCParameterControlView : NSView <VSCParameterControlViewProtocol> {
	VSCSParameter::KeyList keyList;
	VSCSParameter::KeyLabelMap keyLabelMap;
	VSCSParameter::KeyRangeMap keyRangeMap;
	id<VSCParameterListenerProtocol> listener;
}

@property (assign) id<VSCParameterListenerProtocol> listener;


@end
