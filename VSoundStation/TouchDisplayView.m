//
//  TouchDisplayView.m
//  VSoundStation
//
//  Created by Jonathan Thorpe on 03/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "TouchDisplayView.h"
#import "Touch.h"
#import "NetworkDefinitions.h"


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
		self.touchColours = [NSArray arrayWithObjects:[NSColor blueColor], [NSColor greenColor], 
							 [NSColor redColor], [NSColor darkGrayColor], [NSColor blackColor], nil];
		dotRadius = 5.0;
		//currentColorIndex = 0;
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
	for (Touch* touch in [touches allValues]) {
		
		//NSLog(@"Drawing touch %@", touch);
		
		CGFloat touchX = size.width*(touch.xp+0.5);
		CGFloat touchY = size.height*(touch.yp+0.5);
		
		//NSLog(@"x=%.4f, y=%,4f", touchX, touchY);
		
		NSColor* touchColour = [touchColours objectAtIndex:touch.colourIndex]; 
		
//		if (!touchColours || [touchColours count] < 1) 
//			touchColour = [NSColor blueColor];
//		else 
//			touchColour = [touchColours objectAtIndex:count%[touchColours count]];
		
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

-(void) updateTouchesWithJSONArray:(NSArray*)touchDictArray {
	
	NSMutableArray* newTouchKeys = [[NSMutableArray alloc] initWithCapacity:[touchDictArray count]];
	
	for (NSDictionary* touchDict in touchDictArray) {
		
		NSString* touchId = [touchDict objectForKey:[NSString stringWithUTF8String:JSON_TOUCH_ID_KEY]];
		[newTouchKeys addObject:touchId];
		
		Touch* touch = [touches objectForKey:touchId];
		
		if (!touch) {
			touch = [[Touch alloc] init];
			//touch.colourIndex = currentColorIndex;
			[touch setWithJSONDict:touchDict];
			[touches setObject:touch forKey:touchId];
			//currentColorIndex = (currentColorIndex + 1) % [touchColours count];
		}
		
		else {
			[touch setWithJSONDict:touchDict];
		}
		
	}
	
	// Remove touches that are no longer active
	
	NSArray* touchKeys = [touches allKeys];
	
	for (NSString* touchKey in touchKeys) {
		if ([newTouchKeys indexOfObject:touchKey] == NSNotFound) {
			[touches removeObjectForKey:touchKey];
		}
	}
	
	//currentColorIndex = [touches count] % [touchColours count];
	
	[newTouchKeys release];
	
	[self assignTouchDisplayColours];
	
	
}

-(void) assignTouchDisplayColours {
	
	int numberOfColours = (int)[touchColours count];
	
	int colourCounts[numberOfColours];
	
	memset(colourCounts, 0, sizeof(int)*numberOfColours);
	
	NSArray* touchValues = [touches allValues];
	
	for (Touch* touch in touchValues) {
		if (touch.colourIndex != -1 && touch.colourIndex < numberOfColours) 
			colourCounts[touch.colourIndex]++;
	}
	
		
	for (Touch* touch in touchValues) {
		if (touch.colourIndex == -1) {
			int i = 0;
			bool done = false;
			while (!done && i < numberOfColours-1) {
				if (colourCounts[i] < colourCounts[i+1] || colourCounts[i] == 0) {
					touch.colourIndex = i;
					colourCounts[i]++;
					done = true;
				}
				i++;
			}
			if (!done) {
				touch.colourIndex = numberOfColours-1;
				colourCounts[numberOfColours-1]++;
			}
		}
	}

	
	
}

@end
