//
//  DisplayView.m
//  VSoundStation
//
//  Created by Jonathan Thorpe on 19/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "DisplayView.h"


@implementation DisplayView

@synthesize viewId;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		//[self setBa] [NSColor grayColor];
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

-(NSString*) description {
	
	return [NSString stringWithFormat:@"%@ with viewId: %@", [super description], viewId];
	
}

@end
