//
//  TouchDisplayView.m
//  VSoundStation
//
//  Created by Jonathan Thorpe on 03/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "TouchDisplayView.h"
#import "Touch.h"


static CGColorRef CGColorCreateFromNSColor (CGColorSpaceRef colorSpace, NSColor *color) {
	
	NSColor *deviceColor = [color colorUsingColorSpaceName:NSDeviceRGBColorSpace];
	CGFloat components[4];
	[deviceColor getRed:&components[0] green:&components[1] blue:&components[2] alpha:&components[3]];
	return CGColorCreate (colorSpace, components);
	
	return NULL;
	
}



@implementation TouchDisplayView

@synthesize touches, dotRadius, touchColours;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		self.touches = [[[NSMutableDictionary alloc] initWithCapacity:4] autorelease];
		self.touchColours = [NSArray arrayWithObjects:[NSColor blueColor], [NSColor greenColor], [NSColor redColor], nil];
		dotRadius = 5.0;
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
	
	[super drawRect:dirtyRect];
	
	CGSize size = self.bounds.size;
	CGContextRef ctx = [[NSGraphicsContext currentContext] graphicsPort];
	
	
	// Background
	CGMutablePathRef backgroundPath = CGPathCreateMutable(); 
	
	CGPathMoveToPoint(backgroundPath, NULL, 0.0f, 0.0f);
	CGPathAddLineToPoint(backgroundPath, NULL, size.width, 0.0f); 
	CGPathAddLineToPoint(backgroundPath, NULL, size.width, size.height); 
	CGPathAddLineToPoint(backgroundPath, NULL, 0.0, size.height); 
	CGPathCloseSubpath(backgroundPath); 
	
	
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB ();
	CGColorRef cgGrayColor = CGColorCreateFromNSColor (colorSpace, [NSColor grayColor]);
	CGColorRef cgBlackColor = CGColorCreateFromNSColor (colorSpace, [NSColor blackColor]);
	
	CGContextSetFillColorWithColor(ctx, cgGrayColor);  
	//CGContextSetRGBFillColor (ctx, 0.5, 0.5, 0.5, 1);
	CGContextSetLineWidth(ctx, 3);
	CGContextSetStrokeColorWithColor(ctx, cgBlackColor);
	//CGContextSetRGBStrokeColor (ctx, 0.0, 0.0, 0.0, 1);
	CGContextAddPath(ctx, backgroundPath);
	CGContextFillPath(ctx);
	
	CGColorRelease (cgGrayColor);
	CGColorRelease (cgBlackColor);
	CGPathRelease(backgroundPath);
	
	
	NSInteger count = 0;
	for (Touch* touch in touches) {
		
		CGFloat touchX = size.width*touch.xp;
		CGFloat touchY = size.height*touch.yp;
		
		NSColor* touchColour = nil; 
		
		if (!touchColours || [touchColours count] < 1) 
			touchColour = [NSColor blueColor];
		else 
			touchColour = [touchColours objectAtIndex:count%[touchColours count]];
		
		CGColorRef cgTouchColor = CGColorCreateFromNSColor (colorSpace, touchColour);
		
		CGMutablePathRef touchDotPath = CGPathCreateMutable(); 
		
		CGPathAddEllipseInRect(touchDotPath, NULL, 
							   CGRectMake(touchX - (dotRadius/2.0), touchY - (dotRadius/2.0), 2.0*dotRadius, 2.0*dotRadius));
		
		
		CGContextSetFillColorWithColor(ctx, cgTouchColor);  
		//CGContextSetRGBFillColor (ctx, r, g, b, a);
		CGContextSetLineWidth(ctx, 1);
		CGContextSetStrokeColorWithColor(ctx, cgTouchColor);
		CGContextAddPath(ctx, touchDotPath);
		CGContextFillPath(ctx);
		
		CGColorRelease (cgTouchColor);
		CGPathRelease(touchDotPath);
		
		count++;
		
	}
	
	CGColorSpaceRelease (colorSpace);
	
}

@end
