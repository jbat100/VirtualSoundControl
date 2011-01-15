//
//  TouchRelayView.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "TouchRelayView.h"


@implementation TouchRelayView


- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
    // Drawing code
}


- (void)dealloc {
    [super dealloc];
}



- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	
	NSLog(@"======== TOUCHES BEGAN ==========\n\nTouches:\n%@\n\nEvent:\n%@", touches, event);
	
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	
	NSLog(@"======== TOUCHES CANCELLED ==========\n\nTouches:\n%@\n\nEvent:\n%@", touches, event);
	
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	
	NSLog(@"======== TOUCHES ENDED ==========\n\nTouches:\n%@\n\nEvent:\n%@", touches, event);
	
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	
	NSLog(@"======== TOUCHES MOVED ==========\n\nTouches:\n%@\n\nEvent:\n%@", touches, event);
	
}


@end
