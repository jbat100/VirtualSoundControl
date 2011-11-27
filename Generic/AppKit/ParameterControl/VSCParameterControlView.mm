//
//  VSCParameterControlView.mm
//  SynthStation
//
//  Created by Jonathan Thorpe on 27/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCParameterControlView.h"


@implementation VSCParameterControlView

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

-(ParameterKeySet&) parameterKeys {
	return parameterKeys;
}

-(ParameterLabelMap&) parameterLabels {
	return parameterLabels;
}

-(ParameterRangeMap&) parameterRanges {
	return parameterRanges;
}


@end
