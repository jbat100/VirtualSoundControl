//
//  VSCSynthSourceSineWaveControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCSoundParameterView.h"
#import "VSCSynthSourceSine.h"

@interface VSCSynthSourceSineWaveParameterView : VSCSoundParameterView {
    
    VSCSynthSourceSinePtr sourceSine;

}

-(VSCSynthSourceSinePtr) getSourceSine;
-(void) setSourceSine:(VSCSynthSourceSinePtr)_sourceSine;

@end
