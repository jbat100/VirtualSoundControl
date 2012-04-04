//
//  TouchDisplayView.h
//  VSoundStation
//
//  Created by Jonathan Thorpe on 03/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "DisplayView.h"

static CGColorRef CGColorCreateFromNSColor (CGColorSpaceRef colorSpace, NSColor *color);


@interface TouchDisplayView : DisplayView {
	
	NSMutableDictionary* touches;
	CGFloat dotRadius;
	NSArray* touchColours;
	
	//NSUInteger currentColorIndex;
	
	
}

@property (nonatomic, retain) NSMutableDictionary* touches;
@property (nonatomic, assign) CGFloat dotRadius;
@property (nonatomic, retain) NSArray* touchColours;

-(void) updateTouchesWithJSONArray:(NSArray*)touchArray;
-(void) assignTouchDisplayColours;

@end
