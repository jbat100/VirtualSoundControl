//
//  VSCSlidersControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCParamaterControlSlidersView.h"


@implementation VSCParamaterControlSlidersView


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



-(void) updateInterface {
	
	if ([controllerMatrix numberOfColumns] < 1) {
		
	}
	
	
	
}

@end
