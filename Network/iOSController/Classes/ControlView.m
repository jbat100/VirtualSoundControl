//
//  RelayView.m
//  iOSController
//
//  Created by Jonathan Thorpe on 09/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "ControlView.h"
#import "NetworkDefinitions.h"
#import "ControlViewSetup.h"


@implementation ControlView

@synthesize jsonDescriptionDictionary, setup;

#pragma mark -
#pragma mark UIView Methods

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		
		CGRect f = frame;
		f.origin.x = f.origin.y = 0;
		backgroundImageView = [[UIImageView alloc] initWithFrame:f];
		backgroundImageView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
		self.autoresizesSubviews = YES;
		[self addSubview:backgroundImageView];
		[self sendSubviewToBack:backgroundImageView];
        
		// Initialization code here.
		
		jsonDescriptionDictionary = [[NSMutableDictionary alloc] initWithCapacity:10];
		
		[jsonDescriptionDictionary setObject:[NSString stringWithFormat:@"%x", self] 
									  forKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_ID_KEY]];
    
	}
    return self;
}

- (void)drawRect:(CGRect)dirtyRect {
    // Drawing code here.
}

-(void) dealloc {
	
	[(ControlView*)setup setView:nil];
	
	[backgroundImageView release];
	[jsonDescriptionDictionary release];
	
	[super dealloc];
}

#pragma mark -
#pragma mark JSON Methods

-(void) updateJSONDescriptionDictionary {

	if ([setup respondsToSelector:@selector(descriptor)] && 
		[[setup description] isKindOfClass:[NSString class]]) {
			[jsonDescriptionDictionary setObject:[setup description]
										  forKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_DESCRIPTOR_KEY]];		
	}
	else {
		[jsonDescriptionDictionary removeObjectForKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_DESCRIPTOR_KEY]];
	}
	
	
	if ([setup respondsToSelector:@selector(backgroundImageName)] &&
		[[(ControlViewSetup*)setup backgroundImageName] isKindOfClass:[NSString class]]){
		[jsonDescriptionDictionary setObject:[(ControlViewSetup*)setup backgroundImageName]
									  forKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_BACKGROUND_IMAGE_KEY]];
	}
	else {
		[jsonDescriptionDictionary removeObjectForKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_BACKGROUND_IMAGE_KEY]];
	}
	
}

#pragma mark -
#pragma mark Setup Methods

-(void) applySetup {
	
	if ([setup respondsToSelector:@selector(backgroundImageName)]  && 
		[[(ControlViewSetup*)setup backgroundImageName] isKindOfClass:[NSString class]]) {
		backgroundImageView.image = [UIImage imageNamed:[(ControlViewSetup*)setup backgroundImageName]];
	}
	
	else {
		backgroundImageView.image = nil;
	}
	
	if ([setup respondsToSelector:@selector(normalisedFrame)]) {
			
		CGRect normalisedFrame = [(ControlViewSetup*)setup normalisedFrame];
		CGRect parentFrame = [self superview].frame;
		
		CGRect f = CGRectMake(normalisedFrame.origin.x * parentFrame.size.width, 
							  normalisedFrame.origin.y * parentFrame.size.height, 
							  normalisedFrame.size.width * parentFrame.size.width, 
							  normalisedFrame.size.height * parentFrame.size.height);
		
		self.frame = f;
	}

	
}

#pragma mark -
#pragma mark Custom Setters Methods

-(void) setSetup:(NSObject*)s {
	
	if (![s conformsToProtocol:@protocol(ControlViewSetup)]) {
		[NSException raise:@"Invalid setup object" format:@"%@ does not conform to <ControlViewSetup>", s];
		setup = nil;
	}
	
	else {
		setup = s;
	}

	
}

@end
