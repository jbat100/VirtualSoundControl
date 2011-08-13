//
//  EnveloppeView.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCEnveloppeView.h"
#import "VSCEnveloppeViewSetup.h"
#import "VSCColour.h"
#import "CGColorOperations.h"
#import "NSGeomOperations.h"

#import <math.h>



@implementation VSCEnveloppeView


- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.

    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
	
	if (!_enveloppe) {
		// draw whatever is needed to show that no enveloppe is associated with the view
		return;
	}
	
	CGSize size = self.bounds.size;
	
	NSGraphicsContext * nsGraphicsContext = [NSGraphicsContext currentContext]; 
	CGContextRef ctx = (CGContextRef) [nsGraphicsContext graphicsPort];
	
	CGFloat radius = (CGFloat)(_enveloppeViewSetup->getControlPointRadius());
	
	CGColorRef cgSelectedColourRef = CGColorCreateFromVSCColour(_enveloppeViewSetup->getControlPointSelectedColour());
	CGColorRef cgUnselectedColourRef = CGColorCreateFromVSCColour(_enveloppeViewSetup->getControlPointUnselectedColour());
	
	CGColorRef cgLineColorRef = CGColorCreateFromVSCColour(_enveloppeViewSetup->getLineColour());
	
	std::list<VSCEnveloppePoint*> allPoints = _enveloppe->getAllPoints();	
	
	POINT_ITERATOR currentIt;
	POINT_ITERATOR nextIt;
	
	for (POINT_ITERATOR it = allPoints.begin(); it != allPoints.end(); it++) {
		
		nextIt = it;
		nextIt++;
		
		if (nextIt != allPoints.end()) {
			
			VSCEnveloppePoint* currentPoint = *it;
			VSCEnveloppePoint* nextPoint = *nextIt;
			
			// draw line between this point and next
			
			NSPoint point1 = [self pointForEnveloppePoint:currentPoint];
			NSPoint point2 = [self pointForEnveloppePoint:nextPoint];
			
			// Background
			CGMutablePathRef linePath = CGPathCreateMutable(); 
			
			CGPathMoveToPoint(linePath, NULL, point1.x, point1.y);
			CGPathAddLineToPoint(linePath, NULL, point2.x, point2.x); 
			CGContextSetLineWidth(ctx, 2);
			CGContextSetStrokeColorWithColor(ctx, cgLineColorRef);
			CGContextAddPath(ctx, linePath);
			CGContextStrokePath(ctx);
			
		}
		
	}
	
	for (POINT_ITERATOR it = allPoints.begin(); it != allPoints.end(); it++) {
		
		VSCEnveloppePoint* currentPoint = *it;
		
		// draw control circle for point
		
		NSPoint p = [self pointForEnveloppePoint:currentPoint];
		
		CGColorRef cgColor = NULL;
		
		if (_currentlySelectedPoints.find(currentPoint) != _currentlySelectedPoints.end())
			cgColor = cgSelectedColourRef;
		else 
			cgColor = cgUnselectedColourRef;
		
		CGMutablePathRef dotPath = CGPathCreateMutable(); 
		CGPathAddEllipseInRect(dotPath, NULL, CGRectMake(p.x - radius, p.y - radius, 2.0*radius, 2.0*radius));
		CGContextSetFillColorWithColor(ctx, cgColor);  
		CGContextSetLineWidth(ctx, 1);
		CGContextAddPath(ctx, dotPath);
		CGContextFillPath(ctx);
		CGPathRelease(dotPath);
		
	}
	
	
	
}

-(double) valueForPoint:(NSPoint)point {
	
	double normalisedY = 1.0 - (point.y / self.frame.size.height);
	double range = _enveloppeViewSetup->getMaxValue() - _enveloppeViewSetup->getMinValue(); 
	double adjustedY = _enveloppeViewSetup->getMinValue() + (normalisedY*range);
	
	if (_enveloppeViewSetup->getDisplayType() == VSCEnveloppeDisplayTypeDB)
		return pow(10.0, adjustedY) / 10.0;
	
	return adjustedY;
	
}

-(NSTimeInterval) timeForPoint:(CGPoint)point {
	
	double normalisedX = 1.0 - (point.x / self.frame.size.width);
	double range = _enveloppeViewSetup->getMaxTime() - _enveloppeViewSetup->getMinTime(); 
	return (NSTimeInterval)(_enveloppeViewSetup->getMinValue() + (normalisedX*range));
	
}

-(NSPoint) pointForEnveloppePoint:(VSCEnveloppePoint*)enveloppePoint {
	return [self pointForTime:(NSTimeInterval)enveloppePoint->getTime() value:(double)enveloppePoint->getValue()];
}

-(NSPoint) pointForTime:(NSTimeInterval)time value:(double)value {
	
	double timeRange = _enveloppeViewSetup->getMaxTime() - _enveloppeViewSetup->getMinTime(); 
	double timePerPixel = timeRange / self.frame.size.width; 
	double x = (time - _enveloppeViewSetup->getMinTime()) / timePerPixel;
	
	double valueRange = _enveloppeViewSetup->getMaxValue() - _enveloppeViewSetup->getMinValue(); 
	double valuePerPixel = valueRange / self.frame.size.height; 
	double y = (value - _enveloppeViewSetup->getMinValue()) / valuePerPixel;
	
	return NSMakePoint(x, y);
}

-(BOOL) point:(NSPoint)p touchesEnveloppePoint:(VSCEnveloppePoint*)enveloppePoint {
	
	float radius = _enveloppeViewSetup->getControlPointRadius();
	
	/* First do a simple sqaure test to discard faster */
	NSPoint envP = [self pointForEnveloppePoint:enveloppePoint];
	CGRect testRect = CGRectMake(envP.x-radius, envP.y, radius*2, radius*2);
	if (!CGRectContainsPoint(testRect, envP))
		return NO;
	
	/* If first test succeeded, do proper test */
	CGFloat distanceToPoint = DistanceBetweenCGPoints(p, envP);
	if (distanceToPoint < radius) 
		return YES;
	
	return NO;
	
}

-(VSCEnveloppePoint*) enveloppePointForPoint:(NSPoint)point {
	
	std::list<VSCEnveloppePoint*> allPoints = _enveloppe->getAllPoints();	
	
	for (POINT_ITERATOR it = allPoints.begin(); it != allPoints.end(); it++) {
		if ([self point:point touchesEnveloppePoint:(*it)])
			return (*it);
	}
	
	return NULL;
	
}

-(std::list<VSCEnveloppePoint*>) enveloppePointsInRect:(NSRect)rect {
	
	std::list<VSCEnveloppePoint*> allPoints = _enveloppe->getAllPoints();
	std::list<VSCEnveloppePoint*> rectPoints;
	
	for (POINT_ITERATOR it = allPoints.begin(); it != allPoints.end(); it++) {
		NSPoint p = [self pointForEnveloppePoint:(*it)];
		if (CGRectContainsPoint(rect, p))
			rectPoints.push_back(*it);
	}
	
	return rectPoints;
	
}

-(void) setEnveloppePoint:(VSCEnveloppePoint*)controlPoint withPoint:(NSPoint)p {
	controlPoint->setValue([self valueForPoint:p]);
	controlPoint->setTime([self timeForPoint:p]);
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
