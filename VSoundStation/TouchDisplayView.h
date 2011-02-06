//
//  TouchDisplayView.h
//  VSoundStation
//
//  Created by Jonathan Thorpe on 03/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>


static CGColorRef CGColorCreateFromNSColor (CGColorSpaceRef colorSpace, NSColor *color);


@interface TouchDisplayView : NSView {
	
	NSMutableDictionary* touches;
	CGFloat dotRadius;
	NSArray* touchColours;
	
	
}

@property (nonatomic, retain) NSMutableDictionary* touches;
@property (nonatomic, assign) CGFloat dotRadius;
@property (nonatomic, retain) NSArray* touchColours;

@end
