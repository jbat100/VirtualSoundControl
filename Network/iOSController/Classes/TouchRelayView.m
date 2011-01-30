//
//  TouchRelayView.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "TouchRelayView.h"


@implementation TouchRelayView

@synthesize currentTouches;


- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		currentTouches = [[NSMutableArray alloc] initWithCapacity:4];
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
    // Drawing code
}


- (void)dealloc {
	[currentTouches release];
    [super dealloc];
}



- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	
	//NSLog(@"======== TOUCHES BEGAN ==========\n\nTouches:\n%@\n\nEvent:\n%@", touches, event);
	
	NSLog(@"======== TOUCHES BEGAN ==========");
	NSInteger touchCount = 0;
	for (UITouch* touch in [touches allObjects]) {
		touchCount++;
		NSLog(@"Touch %d at 0x%x", touchCount, touch);
	}
	
	[currentTouches addObjectsFromArray:[touches allObjects]];
	

	
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	
	//NSLog(@"======== TOUCHES CANCELLED ==========\n\nTouches:\n%@\n\nEvent:\n%@", touches, event);
	
	//NSLog(@"======== TOUCHES CANCELLED ==========");
	NSInteger touchCount = 0;
	for (UITouch* touch in [touches allObjects]) {
		touchCount++;
		//NSLog(@"Touch %d at 0x%x", touchCount, touch);
		
		[currentTouches removeObjectIdenticalTo:touch];
		
	}
	
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	
	//NSLog(@"======== TOUCHES ENDED ==========\n\nTouches:\n%@\n\nEvent:\n%@", touches, event);
	
	//NSLog(@"======== TOUCHES ENDED ==========");
	NSInteger touchCount = 0;
	for (UITouch* touch in [touches allObjects]) {
		touchCount++;
		//NSLog(@"Touch %d at 0x%x", touchCount, touch);
		
		[currentTouches removeObjectIdenticalTo:touch];
	}
	
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	
	//NSLog(@"======== TOUCHES MOVED ==========\n\nTouches:\n%@\n\nEvent:\n%@", touches, event);
	
	//NSLog(@"======== TOUCHES MOVED ==========");
	NSInteger touchCount = 0;
	for (UITouch* touch in [touches allObjects]) {
		touchCount++;
		//NSLog(@"Touch %d at 0x%x", touchCount, touch);
	}
	
}


@end
