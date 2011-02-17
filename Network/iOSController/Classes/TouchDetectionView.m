//
//  TouchDetectView.m
//  iOSController
//
//  Created by Jonathan Thorpe on 16/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "TouchDetectionView.h"
#import "Definitions.h"

@implementation TouchDetectionView


- (id)initWithFrame:(CGRect)frame {
    
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code.
		
		self.backgroundColor = [UIColor redColor];
		
		self.currentTouches = [[[NSMutableArray alloc] initWithCapacity:MAX_CONCURRENT_TOUCHES] autorelease];
		
		self.multipleTouchEnabled = YES;
		
		
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code.
}
*/

- (void)dealloc {
	self.currentTouches = nil;
    [super dealloc];
}



#pragma mark -
#pragma mark Touch Detection

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	
	//NSLog(@"======== TOUCHES BEGAN ==========\n\nTouches:\n%@\n\nEvent:\n%@", touches, event);
	
	//NSLog(@"======== %d TOUCHES BEGAN ==========", [touches count]);
	NSInteger touchCount = 0;
	for (UITouch* touch in [touches allObjects]) {
		touchCount++;
		//NSLog(@"Touch %d at 0x%x", touchCount, touch);
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
	
	//NSLog(@"======== %d TOUCHES MOVED ==========", [touches count]);
	NSInteger touchCount = 0;
	for (UITouch* touch in [touches allObjects]) {
		touchCount++;
		//NSLog(@"Touch %d at 0x%x", touchCount, touch);
	}
	
}


@end
