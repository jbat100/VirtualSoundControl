//
//  VSCSynthSourceSineWaveControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSynthSourceSineWaveControlView.h"
#import "VSCSoundApple.h"


@implementation VSCSynthSourceSineWaveControlView


- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

-(void) customInit {
	
	[super customInit];
	
	[parameterKeys addObject:[VSCSoundApple VSCSoundFrequencyParameterKey]];
	
	
}

@end
