//
//  TouchRelayView.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "TouchControlView.h"

#define MAX_CONCURRENT_TOUCHES 4


@implementation TouchControlView

@synthesize currentTouches, xChannel, yChannel;


- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		
		currentTouches = [[NSMutableArray alloc] initWithCapacity:MAX_CONCURRENT_TOUCHES];
		xChannel = 1;
		yChannel = 2;
		
		// Instantiate the touch array once and for all...
		NSMutableArray* jsonTouchArray = [[NSMutableArray alloc] initWithCapacity:MAX_CONCURRENT_TOUCHES];
		// jsonDescriptionDictionary should already be allocated in super 
		[jsonDescriptionDictionary setObject:jsonTouchArray 
									  forKey:[NSString stringWithUTF8String:JSON_TOUCH_ARRAY_KEY]];
		
		
		jsonTouchDictionaries = [[NSMutableArray alloc] initWithCapacity:MAX_CONCURRENT_TOUCHES];
		
		for (int i = 0; i<MAX_CONCURRENT_TOUCHES; i++) {
			NSMutableDictionary* dict = [[NSMutableDictionary alloc] initWithCapacity:6];
			[jsonTouchDictionaries addObject:dict];
		}
		
		self.multipleTouchEnabled = YES;
		
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


-(void) updateJSONDescriptionDictionary; {
	
	NSMutableArray* jsonTouchArray = [jsonDescriptionDictionary objectForKey:
									  [NSString stringWithUTF8String:JSON_TOUCH_ARRAY_KEY]];
	
	[jsonTouchArray removeAllObjects];
	
	for (int = 0; i < [currentTouches count]; i++) {
		
		if (i => MAX_CONCURRENT_TOUCHES)
			break;
		
		UITouch* touch = [currentTouches objectAtIndex:i];
		NSMutableDictionary* dict = [jsonTouchDictionaries objectAtIndex:i];
		[touch fillJSONDictionary:dict];
		[jsonTouchArray addObject:dict];
		
	}
	
}


#pragma mark -
#pragma mark Touch Registering

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
