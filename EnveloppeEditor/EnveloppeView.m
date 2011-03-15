//
//  EnveloppeView.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "EnveloppeView.h"
#import "EnveloppeViewSetup.h"

#import <math.h>
#import "Geom.h"



@implementation EnveloppeView

@synthesize enveloppe, enveloppeViewSetup, currentlySelectedPoints;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		self.currentlySelectedPoints = [NSMutableArray arrayWithCapacity:50]; 
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
	
	CGSize size = self.bounds.size;
	CGContextRef ctx = [[NSGraphicsContext currentContext] graphicsPort];
	CGFloat radius = (CGFloat)enveloppeViewSetup.controlPointRadius;
	
	if ([enveloppe.controlPoints count] > 1) {
		for (NSInteger count = 1; count < [enveloppe.controlPoints count]; count++) {
			
			CGColorRef cgColor = enveloppeViewSetup.unselectedControlPointColor;
			
			EnveloppeControlPoint* controlPoint1 = [enveloppe.controlPoints objectAtIndex:count-1];
			EnveloppeControlPoint* controlPoint2 = [enveloppe.controlPoints objectAtIndex:count];
			
			NSPoint point1 = [self pointForControlPoint:controlPoint1];
			NSPoint point2 = [self pointForControlPoint:controlPoint2];
			
			// Background
			CGMutablePathRef linePath = CGPathCreateMutable(); 
			
			CGPathMoveToPoint(linePath, NULL, point1.x, point1.y);
			CGPathAddLineToPoint(linePath, NULL, point2.x, point2.x); 
			
			CGContextSetLineWidth(ctx, 2);
			CGContextSetStrokeColorWithColor(ctx, cgColor);
			CGContextAddPath(ctx, backgroundPath);
			CGContextStrokePath(ctx);
		}
	}
	
	
	for (EnveloppeControlPoint* controlPoint in enveloppe.controlPoints) {
		
		NSPoint p = [self pointForControlPoint:controlPoint];
		
		CGColorRef cgColor = NULL;
		
		if ([currentlySelectedPoints indexOfObjectIdenticalTo:controlPoint] == NSNotFound) 
			cgColor = enveloppeViewSetup.selectedControlPointColor;
		else 
			cgColor = enveloppeViewSetup.unselectedControlPointColor;
		
		CGMutablePathRef dotPath = CGPathCreateMutable(); 
		
		CGPathAddEllipseInRect(dotPath, NULL, 
							   CGRectMake(p.x - radius, p.y - radius, 2.0*radius, 2.0*radius));
		
		
		CGContextSetFillColorWithColor(ctx, cgColor);  
		CGContextSetLineWidth(ctx, 1);
		CGContextAddPath(ctx, touchDotPath);
		CGContextFillPath(ctx);
		CGPathRelease(dotPath);
		
	}
	
}

-(double) valueForPoint:(NSPoint)point {
	
	double normalisedY = 1.0 - (point.y / self.frame.size.height);
	double range = enveloppeViewSetup.maxValue - enveloppeViewSetup.minValue; 
	double adjustedY = enveloppeViewSetup.minValue + (normalisedY*range);
	
	if (enveloppeViewSetup.dB)
		return pow(10.0, adjustedY) / 10.0;
	
	return adjustedY;
	
}

-(NSTimeInterval) timeForPoint:(CGPoint)point {
	
	double normalisedX = 1.0 - (point.x / self.frame.size.width);
	double range = enveloppeViewSetup.maxTime - enveloppeViewSetup.minTime; 
	return (NSTimeInterval)(enveloppeViewSetup.minValue + (normalisedX*range));
	
}

-(NSPoint) pointForControlPoint:(EnveloppeControlPoint*)controlPoint {
	return [self pointForTime:controlPoint.timeStamp value:controlPoint.value];
}

-(NSPoint) pointForTime:(NSTimeInterval)time value:(double)value {
	
	double timeRange = enveloppeViewSetup.maxTime - enveloppeViewSetup.minTime; 
	double timePerPixel = timeRange / self.frame.size.width; 
	double x = (time - enveloppeViewSetup.minTime) / timePerPixel;
	
	double valueRange = enveloppeViewSetup.maxValue - enveloppeViewSetup.minValue; 
	double valuePerPixel = valueRange / self.frame.size.height; 
	double y = (value - enveloppeViewSetup.minValue) / valuePerPixel;
	
	return NSMakePoint(x, y);
}

-(EnveloppeControlPoint*) controlPointForPoint:(NSPoint)point {
	
	for (EnveloppeControlPoint* controlPoint in enveloppe.controlPoints) {
		
		NSPoint p = [self pointForTime:controlPoint.time value:controlPoint.value];
		CGFloat distanceToPoint = DistanceBetweenCGPoints(p, point);
		
		if (distanceToPoint < enveloppeViewSetup.controlPointRadius) 
			return controlPoint;
		
	}
	
	return nil;
	
}

-(NSArray*) controlPointsInRect:(NSRect)rect {
	
	NSMutableArray* controlPoints = [NSMutableArray arrayWithCapacity:[[enveloppe.controlPoints] count]];
	
	for (EnveloppeControlPoint* controlPoint in enveloppe.controlPoints) {
		NSPoint p = [self pointForControlPoint:controlPoint];
		if (NSPointInRect(p, rect))
			[controlPoints addObject:controlPoint];
	}
	
	return [NSArray arrayWithArray:controlPoints];
	
}

-(void) setControlPoint:(EnveloppeControlPoint*)controlPoint withPoint:(NSPoint)p {
	controlPoint.value = [self valueForPoint:p];
	controlPoint.timeStamp = [self timeForPoint:p];
}

#pragma mark -
#pragma mark Mouse Input

-(BOOL) acceptsFirstMouse:(NSEvent*)event {
	return YES;
}

-(void) mouseDown:(NSEvent*)event {
	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:self];
    [self setNeedsDisplay:YES];
	
	NSLog(@"Mouse down at x: %f, y: %f", locationInView.x, locationInView.y);
	
	/*
	NSAlphaShiftKeyMask
	NSShiftKeyMask      
	NSControlKeyMask    
	NSAlternateKeyMask  
	NSCommandKeyMask    
	NSNumericPadKeyMask 
	NSHelpKeyMask       
	NSFunctionKeyMask   
	 */
	
	NSUInteger modifierFlags = [event modifierFlags];
	
	BOOL foundModifier = NO;
	
	if (modifierFlags & NSShiftKeyMask) {
		NSLog(@"With shift");
		foundModifier = YES;
		performingSelection = YES;
	}
	if (modifierFlags & NSAlphaShiftKeyMask) {
		NSLog(@"With alpha shift");
		foundModifier = YES;
	}
	if (modifierFlags & NSControlKeyMask) {
		NSLog(@"With control");
		foundModifier = YES;
	}
	if (modifierFlags & NSAlternateKeyMask) {
		NSLog(@"With alternate");
		foundModifier = YES;
	}
	if (modifierFlags & NSCommandKeyMask) {
		NSLog(@"With command");
		foundModifier = YES;
	}
	
	if (!foundModifier) {
		[currentlySelectedPoints removeAllObjects];
		EnveloppeControlPoint* controlPoint = [self controlPointForPoint:locationInView];
		if (!controlPoint) {
			double v = [self valueForPoint:locationInView];
			NSTimeInterval t = [self timeForPoint:locationInView];
			controlPoint = [EnveloppeControlPoint controlPointWithValue:v andTimeStamp:t];
		}
		[currentlySelectedPoints addObject:controlPoint];
	}
	
	[self setNeedsDisplay:YES];
	
	
}

-(void) mouseUp:(NSEvent *)event {
	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:self];
    [self setNeedsDisplay:YES];
	
	NSLog(@"Mouse down at x: %f, y: %f", locationInView.x, locationInView.y);
	
	performingSelection = NO;
	
}

-(void) mouseDragged:(NSEvent *)event {
	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:self];
	
	CGFloat deltaX = [event deltaX];
	CGFloat deltaY = [event deltaY];
	
	if (performingSelection) {
		
		// current values 
		CGFloat cx = selectionRect.origin.x;
		CGFloat cy = selectionRect.origin.y;		
		// new values
		CGFloat nx;
		CGFloat ny;
		CGFloat nw;
		CGFloat nh;
		
		if (locationInView.x < cx) {
			nx = locationInView.x;
			nw = cx - locationInView.x;
		}
		else {
			nx = cx;
			nw = locationInView.x - cx;
		}
		
		if (locationInView.y < cy) {
			ny = locationInView.y;
			nh = cy - locationInView.y;
		}
		else {
			ny = cy;
			nh = locationInView.y - cy;
		}
		
		selectionRect = NSMakeRect(nx, ny, nw, nh);
		self.currentlySelectedPoints = [self controlPointsInRect:selectionRect];
	}
	
	if (!performingSelection) {
		for (EnveloppeControlPoint* controlPoint in currentlySelectedPoints) {
			NSPoint p = [self pointForControlPoint:controlPoint];
			p.x += deltaX;
			p.y += deltaY;
			[self setControlPoint:controlPoint withPoint:p];
		}
	}
	
	[self setNeedsDisplay:YES];
	
}

@end
