//
//  RelayView.m
//  iOSController
//
//  Created by Jonathan Thorpe on 09/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "ControlView.h"


@implementation ControlView

@synthesize descriptor, backgroundImageName;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        
		// Initialization code here.
		
		jsonDescriptionDictionary = [[NSMutableDictionary alloc] initWithCapacity:10];
		
		[jsonDescriptionDictionary setObject:[NSString stringWithFormat:@"%x", self] 
									  forKey:[NSString stringWithUTF8String:JSON_RELAY_VIEW_ID_KEY]];
    
	}
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

-(void) updateJSONDescriptionDictionary; {

	
	
}

#pragma mark -
#pragma mark Custom setters

-(void) setDescriptor:(NSString *)s {
	
	[descriptor release];
	descriptor = [s retain];
	
	if (descriptor)
		[jsonDescriptionDictionary setObject:descriptor
									  forKey:[NSString stringWithUTF8String:JSON_RELAY_VIEW_DESCRIPTOR_KEY]];
	else 
		[jsonDescriptionDictionary removeObjectForKey:[NSString stringWithUTF8String:JSON_RELAY_VIEW_DESCRIPTOR_KEY]];
	
}

-(void) setBackgroundImageName:(NSString*)s {
	
	[backgroundImageName release];
	backgroundImageName = [s retain];
	
	if (backgroundImageName) 
		[jsonDescriptionDictionary setObject:backgroundImageName
									  forKey:[NSString stringWithUTF8String:JSON_RELAY_VIEW_BACKGROUND_IMAGE_KEY]];
	else 
		[jsonDescriptionDictionary removeObjectForKey:[NSString stringWithUTF8String:JSON_RELAY_VIEW_BACKGROUND_IMAGE_KEY]];
	
}

@end
