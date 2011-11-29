//
//  VSCParameterControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 27/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCParameterControlViewProtocol.h"
#import "VSCSoundParameters.h"

#import <map>
#import <set>

@interface VSCParameterControlView : NSView <VSCParameterControlViewProtocol> {
	
	VSCSParameter::KeySet keySet;
	VSCSParameter::KeyLabelMap keyLabelMap;
	VSCSParameter::KeyRangeMap keyRangeMap;
	
	id<VSCParameterControlViewDelegate> delegate;

}

@property (assign) id<VSCParameterControlViewDelegate> delegate;


@end
