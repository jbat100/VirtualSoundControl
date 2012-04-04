//
//  VSCEnveloppeLayer.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCEnveloppeLayer.h"

@implementation VSCEnveloppeLayer


- (void)drawInContext:(CGContextRef)ctx {
    
    drawRectFill(ctx, self.bounds, CGColorCreateFromRGBNSColor([NSColor lightGrayColor]));
    
    
    for (VSCEnveloppe::List::const_iterator it = _enveloppeList.begin(); it != _enveloppeList.end(); ++it) {
        
        VSCEnveloppePtr enveloppe = (*it);
        
        
        
    }
    
    
}

#if 0

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
	
	if (!_enveloppe) {
		// draw whatever is needed to show that no enveloppe is associated with the view
		return;
	}
	
	//NSDrawLog(@"-------------- In %@ drawRect ----------------", self);
	
	//CGSize size = self.bounds.size;
	
	NSGraphicsContext * nsGraphicsContext = [NSGraphicsContext currentContext]; 
	CGContextRef ctx = (CGContextRef) [nsGraphicsContext graphicsPort];
	
	/*
	 *	Draw background
	 */
	drawRectFill(ctx, self.bounds, CGColorCreateFromRGBNSColor([NSColor lightGrayColor]));
	
	/*
	 *	Draw enveloppe
	 */
	
	
	
	/*
	 *	Draw enveloppe
	 */
	
	CGFloat radius = (CGFloat)(_enveloppeViewSetup->getControlPointRadius());
	CGColorRef cgSelectedColourRef = CGColorCreateFromVSCColour(_enveloppeViewSetup->getControlPointSelectedColour());
	CGColorRef cgUnselectedColourRef = CGColorCreateFromVSCColour(_enveloppeViewSetup->getControlPointUnselectedColour());
	CGColorRef cgLineColorRef = CGColorCreateFromVSCColour(_enveloppeViewSetup->getLineColour());
	
    VSCEnveloppe::ConstPointIterator nextIt;
	VSCEnveloppe::ConstPointIterator endIt = _enveloppe->getPointEndIterator();
	
	//NSDrawLog(@"Drawing enveloppe lines, frame is %@", NSStringFromRect([self frame]));
	
	for (VSCEnveloppe::ConstPointIterator it = _enveloppe->getPointBeginIterator(); it !=endIt; it++) {
		
		nextIt = it;
		nextIt++;
		
		if (nextIt != endIt) {
			
			VSCEnveloppePointPtr currentPoint = *it;
			VSCEnveloppePointPtr nextPoint = *nextIt;
			
			// draw line between this point and next
			
			NSPoint point1 = [self pointForEnveloppePoint:currentPoint];
			NSPoint point2 = [self pointForEnveloppePoint:nextPoint];
			
			//NSDrawLog(@"Converted points to %@ and %@", NSStringFromPoint(point1), NSStringFromPoint(point2));
			
			// Background
			CGMutablePathRef linePath = CGPathCreateMutable(); 
			CGPathMoveToPoint(linePath, NULL, point1.x, point1.y);
			CGPathAddLineToPoint(linePath, NULL, point2.x, point2.y); 
			CGContextSetLineWidth(ctx, 2);
			CGContextSetStrokeColorWithColor(ctx, cgLineColorRef);
			CGContextAddPath(ctx, linePath);
			CGContextStrokePath(ctx);
			CGPathRelease(linePath);
			
		}
		
	}
	
	//NSDrawLog(@"Drawing enveloppe points, frame is %@", NSStringFromRect([self frame]));
	
	for (VSCEnveloppe::ConstPointIterator it = _enveloppe->getPointBeginIterator(); it !=endIt; it++) {
		
		VSCEnveloppePointPtr currentPoint = *it;
		
		// draw control circle for point
		
		NSPoint p = [self pointForEnveloppePoint:currentPoint];
		
		//NSDrawLog(@"Converted points to %@", NSStringFromPoint(p));
		
		CGColorRef cgColor = NULL;
		
		/*
		 *	Make a set containing the currently selected point plus the points in the current selection rect
		 */
		
		BOOL isSelected = NO;
		
		if (_currentlySelectedPoints.find(currentPoint) != _currentlySelectedPoints.end()) 
			isSelected = YES;
		else if (_pointsInCurrentSelectionRect.find(currentPoint) != _pointsInCurrentSelectionRect.end())
			isSelected = YES;
		
		if (isSelected) 
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
	
	//NSDrawLog(@"-------------- End %@ drawRect ----------------", self);
	
	if (!CGRectIsNull(currentSelectionRect) && !CGRectIsEmpty(currentSelectionRect)) {
		drawRectOutline(ctx, currentSelectionRect, 1, CGColorCreateFromRGBNSColor([NSColor grayColor]));
	}
	
}

#endif


@end
