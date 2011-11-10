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

static NSArray* sineWaveParameterKeys = nil;
static NSDictionary* sineWaveParameterIndexPaths = nil;
static NSDictionary* sineWaveParameterDisplayStrings = nil;


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
	

	
}

+(NSArray*) sineWaveParameterKeys {
    
    if (sineWaveParameterKeys == nil) {
        sineWaveParameterKeys = [[NSArray arrayWithObjects:
                                 [VSCSoundApple VSCSoundFrequencyParameterKey], 
                                 [VSCSoundApple VSCSoundDBGainParameterKey],
                                 [VSCSoundApple VSCSoundPhaseParameterKey], nil] retain];
    }
    return sineWaveParameterKeys;
}

+(NSDictionary*) sineWaveParameterDisplayStrings {
    return nil;
}

+(NSDictionary*) sineWaveParameterIndexPaths {
    
    if (sineWaveParameterIndexPaths == nil) {
        sineWaveParameterIndexPaths = [[NSDictionary dictionaryWithObjectsAndKeys:
                                       [NSIndexPath indexPathWithIndex:0], [VSCSoundApple VSCSoundFrequencyParameterKey], 
                                       [NSIndexPath indexPathWithIndex:1], [VSCSoundApple VSCSoundDBGainParameterKey], 
                                       [NSIndexPath indexPathWithIndex:2], [VSCSoundApple VSCSoundPhaseParameterKey], 
                                        nil] retain];
    }
    return sineWaveParameterIndexPaths;
}

-(NSDictionary*) parameterIndexPaths {
    return [VSCSynthSourceSineWaveControlView sineWaveParameterIndexPaths];
}

#pragma mark - VSCParameterControlViewDataSource Methods


-(NSString*) parameterControlView:(VSCParameterControlView*)view displayStringForParameterAtIndexPath:(NSIndexPath *)indexPath {
	return [VSCSoundApple displayStringForKey:[self parameterControlView:nil parameterKeyForParameterAtIndexPath:indexPath]];
}

-(float) parameterControlView:(VSCParameterControlView*)view floatForParameterAtIndexPath:(NSIndexPath *)indexPath  {
	return 0.0;
}




@end
