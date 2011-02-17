//
//  TouchRelayView.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "TouchControlView.h"
#import "TouchControlViewSetup.h"
#import "ControlViewSetup.h"

#import "TouchDetectionView.h"

#import "UITouch+JSONPackaging.h"

#import "Definitions.h"
#import "NetworkDefinitions.h"


@implementation TouchControlView


@synthesize touchDetectionView;


- (id)initWithFrame:(CGRect)frame {
	
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		
		self.backgroundColor = [UIColor greenColor];
		
		CGRect f = frame;
		f.origin.x = f.origin.y = 0;
		touchDetectionView = [[TouchDetectionView alloc] initWithFrame:f];
		touchDetectionView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
		self.autoresizesSubviews = YES;
		[self addSubview:touchDetectionView];
		
		
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
		
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
    // Drawing code
}


- (void)dealloc {
	
	[touchDetectionView release];
	[jsonTouchDictionaries release];
	
    [super dealloc];
}


-(void) updateJSONDescriptionDictionary; {
	
	
	
	if ([setup respondsToSelector:@selector(xChannel)]) {
		[jsonDescriptionDictionary setObject:[NSString stringWithFormat:@"%d", [(TouchControlViewSetup*)setup xChannel]] 
									  forKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_TOUCH_X_CHANNEL]];
	}
	else {
		[jsonDescriptionDictionary removeObjectForKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_TOUCH_X_CHANNEL]];
	}
	
	if ([setup respondsToSelector:@selector(xChannel)]) {
		[jsonDescriptionDictionary setObject:[NSString stringWithFormat:@"%d", [(TouchControlViewSetup*)setup xChannel]] 
									  forKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_TOUCH_Y_CHANNEL]];
	}
	else {
		[jsonDescriptionDictionary removeObjectForKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_TOUCH_Y_CHANNEL]];
	}

	
	
	NSMutableArray* jsonTouchArray = [jsonDescriptionDictionary objectForKey:
									  [NSString stringWithUTF8String:JSON_TOUCH_ARRAY_KEY]];
	
	[jsonTouchArray removeAllObjects];
	
	for (int i = 0; i < [touchDetectionView.currentTouches count]; i++) {
		
		if (i >= MAX_CONCURRENT_TOUCHES)
			break;
		
		UITouch* touch = [touchDetectionView.currentTouches objectAtIndex:i];
		NSMutableDictionary* dict = [jsonTouchDictionaries objectAtIndex:i];
		[touch fillJSONDictionary:dict];
		[jsonTouchArray addObject:dict];
		
	}
	
}

#pragma mark -
#pragma mark Custom Setup Setter

-(void) setSetup:(NSObject*)s {
	
	[super setSetup:s];
	
	// if setup != s then the super call failed and that's it...
	if (setup != s) 
		return;
	
	if (![s conformsToProtocol:@protocol(TouchControlViewSetup)]) {
		[NSException raise:@"Invalid setup object" format:@"%@ does not conform to <TouchControlViewSetup>", s];
		setup = nil;
	}
	
}


@end
