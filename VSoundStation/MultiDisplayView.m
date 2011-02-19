//
//  MultiDisplayScrollView.m
//  VSoundStation
//
//  Created by Jonathan Thorpe on 19/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "MultiDisplayView.h"


@implementation MultiDisplayView

@synthesize displayViews;

#pragma mark -
#pragma mark NSView Methods

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		
		horizontalSeparator = 10;
		displayViews = [[NSMutableArray alloc] initWithCapacity:6];
		
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

#pragma mark -
#pragma mark Display Views 

-(void) addDisplayView:(DisplayView*)dispView {
	
	if (![dispView isKindOfClass:[DisplayView class]]) {
		[NSException raise:@"Invalid argument" format:@"Argument not a DisplayView instance"];
		return;
	}
	[displayViews addObject:dispView];

}

-(void) removeDisplayView:(DisplayView*)dispView {
	[displayViews removeObjectIdenticalTo:dispView];
}

-(void) removeDisplayViewWithId:(NSString*)viewId {
	
	DisplayView* dispView = nil;
	
	while ((dispView = [self displayViewWithId:viewId])) 
		[self removeDisplayView:dispView];
	
}

-(void) removeAllDisplayViews {
	[displayViews removeAllObjects];
}

-(DisplayView*) displayViewWithId:(NSString*)viewId {
	
	//NSLog(@"Getting display view with id %@, displayViews is %@", viewId, displayViews);
	
	for (DisplayView* dispView in displayViews) {
		
		if ([[dispView viewId] isEqualToString:viewId])
			return dispView;
	}
	
	return nil;
	
}


#pragma mark -
#pragma mark Display View Layout

-(void) layoutDisplayViews {
	
	NSArray* subviews = [self subviews];
	
	for (NSView* subview in subviews) 
		[subview removeFromSuperview];
	
	CGRect f = CGRectMake(0, 0, self.frame.size.height, self.frame.size.height);
	
	for (DisplayView* dispView in displayViews) {
		dispView.frame = f;
		f.origin.x += f.size.width + horizontalSeparator;
		[self addSubview:dispView];
	}
	
	self.bounds = CGRectMake(0, 0, f.origin.x, f.size.height);
	
}




@end
