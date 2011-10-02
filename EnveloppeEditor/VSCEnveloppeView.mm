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
#import "VSCBoost.h"
#import "VSCException.h"

#import "CGColorOperations.h"
#import "NSGeomOperations.h"

#import <cmath>
#import <assert.h>

@interface VSCEnveloppeView ()

-(void) addPointsInRect:(NSRect)rect toPointSet:(std::set<VSCEnveloppePointPtr>&)pointSet;

@end



@implementation VSCEnveloppeView


- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		_enveloppe = VSCEnveloppePtr(); 
		_enveloppeViewSetup = VSCEnveloppeViewSetupPtr(new VSCEnveloppeViewSetup());
		[self setNeedsDisplay:YES];
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
	
	if (!_enveloppe) {
		// draw whatever is needed to show that no enveloppe is associated with the view
		return;
	}
	
	//NSDrawLog(@"-------------- In %@ drawRect ----------------", self);
	
	CGSize size = self.bounds.size;
	
	NSGraphicsContext * nsGraphicsContext = [NSGraphicsContext currentContext]; 
	CGContextRef ctx = (CGContextRef) [nsGraphicsContext graphicsPort];
	
	drawRectFill(ctx, self.bounds, CGColorCreateFromRGBNSColor([NSColor lightGrayColor]));
	
	CGFloat radius = (CGFloat)(_enveloppeViewSetup->getControlPointRadius());
	CGColorRef cgSelectedColourRef = CGColorCreateFromVSCColour(_enveloppeViewSetup->getControlPointSelectedColour());
	CGColorRef cgUnselectedColourRef = CGColorCreateFromVSCColour(_enveloppeViewSetup->getControlPointUnselectedColour());
	CGColorRef cgLineColorRef = CGColorCreateFromVSCColour(_enveloppeViewSetup->getLineColour());
	
	ConstEnvPntIter nextIt;
	ConstEnvPntIter endIt = _enveloppe->getPointEndIterator();
	
	//NSDrawLog(@"Drawing enveloppe lines, frame is %@", NSStringFromRect([self frame]));
	
	for (ConstEnvPntIter it = _enveloppe->getPointBeginIterator(); it !=endIt; it++) {
		
		nextIt = it;
		nextIt++;
		
		if (nextIt != endIt) {
			
			VSCEnveloppePointPtr currentPoint = *it;
			VSCEnveloppePointPtr nextPoint = *nextIt;
			
#ifdef VSC_DEBUG_COCOA_DRAW
			//std::cout << "Drawing path between " << *currentPoint << " and " << *nextPoint << "\n";
#endif
			
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
	
	for (ConstEnvPntIter it = _enveloppe->getPointBeginIterator(); it !=endIt; it++) {
		
		VSCEnveloppePointPtr currentPoint = *it;
		
#ifdef VSC_DEBUG_COCOA_DRAW
		//std::cout << "Drawing point for " << *currentPoint;
#endif
		
		// draw control circle for point
		
		NSPoint p = [self pointForEnveloppePoint:currentPoint];
		
		//NSDrawLog(@"Converted points to %@", NSStringFromPoint(p));
		
		CGColorRef cgColor = NULL;
		
		if (_currentlySelectedPoints.find(currentPoint) != _currentlySelectedPoints.end()) {
			cgColor = cgSelectedColourRef;
			//NSDrawLog(@"Point is Selected!");
		}
		else {
			cgColor = cgUnselectedColourRef;
		}
		
		CGMutablePathRef dotPath = CGPathCreateMutable(); 
		CGPathAddEllipseInRect(dotPath, NULL, CGRectMake(p.x - radius, p.y - radius, 2.0*radius, 2.0*radius));
		CGContextSetFillColorWithColor(ctx, cgColor);  
		CGContextSetLineWidth(ctx, 1);
		CGContextAddPath(ctx, dotPath);
		CGContextFillPath(ctx);
		CGPathRelease(dotPath);
		
	}
	
	//NSDrawLog(@"-------------- End %@ drawRect ----------------", self);
	
}

#pragma mark - Auto Adjust View Setup

/* 
 * Automatic View Setup
 */
-(void)autoAdjustEnveloppeViewSetup {
    
    assert(_enveloppeViewSetup);
	
	if (!_enveloppe) {
		_enveloppeViewSetup->setToDefault();
	}
    
    double minTime, maxTime, minValue, maxValue;
    
    try {
        minTime = _enveloppe->minTime();
        maxTime = _enveloppe->maxTime();
        minValue = _enveloppe->minValue();
        maxValue = _enveloppe->maxValue();
        
        double timeMargin = std::abs((maxTime-minTime)*0.2); 
        double valueMargin = std::abs((maxValue-minValue)*0.2); 
        
        //minTime -= timeMargin
        maxTime += timeMargin;
        minValue -= valueMargin;
        maxValue += valueMargin;
    } 
    catch (VSCEnveloppeEmptyException& e) {
        minTime = 0.0;
        maxTime = 1.0;
        minValue = -1.0;
        maxValue = 1.0;
    }
    
    if (_enveloppeViewSetup) {
        _enveloppeViewSetup->setMinTime(minTime);
        _enveloppeViewSetup->setMaxTime(maxTime);
        _enveloppeViewSetup->setMinValue(minValue);
        _enveloppeViewSetup->setMaxValue(maxValue);
    }
    
}

#pragma mark - C++ Setters/Getters

-(VSCEnveloppePtr) getEnveloppe {
    return _enveloppe;
}

-(void) setEnveloppe:(VSCEnveloppePtr)enveloppe {
	_currentlySelectedPoints.clear();
	_pointsInCurrentSelectionRect.clear();
    _enveloppe = enveloppe;
	[self setNeedsDisplay:YES];
}

-(VSCEnveloppeViewSetupPtr) getEnveloppeViewSetup {
    return _enveloppeViewSetup;
}

-(void) setEnveloppeViewSetup:(VSCEnveloppeViewSetupPtr)enveloppeViewSetup {
	assert(enveloppeViewSetup);
    _enveloppeViewSetup = enveloppeViewSetup;
}

-(void)getCurrentlySelectedPoints:(std::set<VSCEnveloppePointPtr>&)points {
    for (std::set<VSCEnveloppePointPtr>::iterator it = _currentlySelectedPoints.begin(); it != _currentlySelectedPoints.end(); it++) {
		points.insert(*it);
	}
}

-(void)setCurrentlySelectedPoints:(std::set<VSCEnveloppePointPtr>&)points {
    _currentlySelectedPoints.clear();
    for (std::set<VSCEnveloppePointPtr>::iterator it = points.begin(); it != points.end(); it++) {
		_currentlySelectedPoints.insert(*it);
	}
}


#pragma mark - View to enveloppe conversion tools

-(double) valueDeltaForPointYDelta:(double)pointYDelta {
	double range = _enveloppeViewSetup->getMaxValue() - _enveloppeViewSetup->getMinValue(); 
	return (pointYDelta / (double)self.frame.size.height) * range;
}

-(double) timeDeltaForPointXDelta:(double)pointXDelta {
	double range = _enveloppeViewSetup->getMaxTime() - _enveloppeViewSetup->getMinTime(); 
	return (pointXDelta / (double)self.frame.size.width) * range;
}

-(double) valueForPoint:(NSPoint)point {

	assert(_enveloppeViewSetup);
	
	if (!_enveloppeViewSetup)
		return 0.0;
	double normalisedY = (point.y / (double)self.frame.size.height);
	double range = _enveloppeViewSetup->getMaxValue() - _enveloppeViewSetup->getMinValue(); 
	double adjustedY = _enveloppeViewSetup->getMinValue() + (normalisedY*range);
	
	return adjustedY;
	
}

-(double) timeForPoint:(CGPoint)point {
	
	assert(_enveloppeViewSetup);
	
	if (!_enveloppeViewSetup)
		return 0.0;
	double normalisedX = (point.x / (double)self.frame.size.width);
	double range = _enveloppeViewSetup->getMaxTime() - _enveloppeViewSetup->getMinTime(); 
	return (NSTimeInterval)(_enveloppeViewSetup->getMinTime() + (normalisedX*range));
	
}

-(NSPoint) pointForEnveloppePoint:(VSCEnveloppePointPtr)enveloppePoint {
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

-(BOOL) point:(NSPoint)p touchesEnveloppePoint:(VSCEnveloppePointPtr)enveloppePoint {
	
	assert(_enveloppeViewSetup);
	
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

#pragma mark - Enveloppe points handling

-(VSCEnveloppePointPtr) enveloppePointUnderPoint:(NSPoint)point {
	
	if (!_enveloppe) {
		return VSCEnveloppePointPtr();
	}
	
	for (ConstEnvPntIter it = _enveloppe->getPointBeginIterator(); it !=_enveloppe->getPointEndIterator(); it++) {
		if ([self point:point touchesEnveloppePoint:(*it)])
			return (*it);
	}
	
	return VSCEnveloppePointPtr();
	
}

-(void) getEnveloppePoints:(std::list<VSCEnveloppePointPtr>&)ps InRect:(NSRect)rect {
	
	if (!_enveloppe) 
		return;
	
	for (ConstEnvPntIter it = _enveloppe->getPointBeginIterator(); it !=_enveloppe->getPointEndIterator(); it++) {
		NSPoint p = [self pointForEnveloppePoint:(*it)];
		if (CGRectContainsPoint(rect, p))
			ps.push_back(*it);
	}
	
}

-(void) setEnveloppePoint:(VSCEnveloppePointPtr)controlPoint withPoint:(NSPoint)p {
	controlPoint->setValue([self valueForPoint:p]);
	controlPoint->setTime([self timeForPoint:p]);
}

-(VSCEnveloppePointPtr) createEnveloppePointForPoint:(NSPoint)point {
	
	double t = [self timeForPoint:point];
	
	if (t<0.0) 
		return VSCEnveloppePointPtr();
	
	return VSCEnveloppePointPtr(new VSCEnveloppePoint([self valueForPoint:point], t));
	
}

-(void) addPointsInRect:(NSRect)rect toPointSet:(std::set<VSCEnveloppePointPtr>&)pointSet {
	
	std::list<VSCEnveloppePointPtr> rectPoints; 
	[self getEnveloppePoints:rectPoints InRect:rect];
	
	for (ConstEnvPntIter it = rectPoints.begin(); it != rectPoints.end(); it++) {
		pointSet.insert(*it);
	}
	
}


#pragma mark - Mouse Input

-(BOOL) acceptsFirstMouse:(NSEvent*)event {
	return YES;
}

-(void) mouseDown:(NSEvent*)event {
	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:[[self window] contentView]];
    [self setNeedsDisplay:YES];
    
    movedSinceMouseDown = NO;
	
	NSLog(@"Mouse down at x: %f, y: %f", locationInView.x, locationInView.y);
	
	if (!_enveloppe) {
		currentMouseAction = VSCEnveloppeViewMouseActionNone;
		return;
	}
	
	VSCEnveloppePointPtr enveloppePoint = [self enveloppePointUnderPoint:locationInView];
	
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
		currentMouseAction = VSCEnveloppeViewMouseActionPersistentSelect;
	}
	else if (modifierFlags & NSAlphaShiftKeyMask) {
		NSLog(@"With alpha shift");
		foundModifier = YES;
	}
	else if (modifierFlags & NSControlKeyMask) {
		NSLog(@"With control");
		foundModifier = YES;
	}
	else if (modifierFlags & NSAlternateKeyMask) {
		NSLog(@"With alternate");
		foundModifier = YES;
	}
	else if (modifierFlags & NSCommandKeyMask) {
		NSLog(@"With command");
		foundModifier = YES;
	}
	else {
		
		if (!enveloppePoint) {
			currentMouseAction = VSCEnveloppeViewMouseAction(VSCEnveloppeViewMouseActionCreate | 
															 VSCEnveloppeViewMouseActionSelect);
		}
		
		else {
			currentMouseAction = VSCEnveloppeViewMouseAction(VSCEnveloppeViewMouseActionDelete | 
															 VSCEnveloppeViewMouseActionMove);
			_currentlySelectedPoints.insert(enveloppePoint);
		}
		
	}
		
	if (currentMouseAction & (VSCEnveloppeViewMouseActionSelect | VSCEnveloppeViewMouseActionPersistentSelect)) {
		currentSelectionRect = NSMakeRect(locationInView.x, locationInView.y, 0.0, 0.0);
		currentSelectionOrigin = NSMakePoint(locationInView.x, locationInView.y);
	}
	
	[self setNeedsDisplay:YES];
	
	
}

-(void) mouseUp:(NSEvent *)event {
	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:[[self window] contentView]];
    [self setNeedsDisplay:YES];
	NSLog(@"Mouse up at x: %f, y: %f", locationInView.x, locationInView.y);
	
	if (!_enveloppe) {
		currentMouseAction = VSCEnveloppeViewMouseActionNone;
		return;
    }
	
    VSCEnveloppePointPtr enveloppePoint = [self enveloppePointUnderPoint:locationInView];
    
	/*
	 *	If the mouse action contains create and the mouse has not moved since click down, then create
	 *	a new point at the click location
	 */
	if ((currentMouseAction & VSCEnveloppeViewMouseActionCreate) && movedSinceMouseDown == NO) {
		NSAssert(!enveloppePoint.get(), @"Expected enveloppePoint to be NULL");
		VSCEnveloppePointPtr newPoint = [self createEnveloppePointForPoint:locationInView];
		_enveloppe->addPoint(newPoint);
	}
	
	/*
	 *	If the mouse action is select or persistant select and the mouse has not moved since click down
	 *	then select/deselect the point at mouse location, if non-persistent select then deselect all other 
	 *	points, reset current selection rect
	 */
	if ((currentMouseAction & (VSCEnveloppeViewMouseActionSelect | VSCEnveloppeViewMouseActionPersistentSelect)) && 
		movedSinceMouseDown == NO) {
		
		if (currentMouseAction & VSCEnveloppeViewMouseActionSelect) {
			_currentlySelectedPoints.clear();
		}
		
		// If a point exists at the point where the click occured then select/deselect the point
		if (enveloppePoint) {
			std::set<VSCEnveloppePointPtr>::iterator iter = _currentlySelectedPoints.find(enveloppePoint);
			// if the point is not selected, add it to selected...
			if (iter == _currentlySelectedPoints.end()) {
				_currentlySelectedPoints.insert(enveloppePoint);
			}
			// else remove it from the currently selected points
			else {
				_currentlySelectedPoints.erase(enveloppePoint);
			}
		}
		
		currentSelectionRect = NSMakeRect(0.0, 0.0, 0.0, 0.0);
		currentSelectionOrigin = NSMakePoint(0.0, 0.0);

	}

	/*
	 *	If the mouse action contains delete and the mouse has not moved since click down, then delete
	 *	the point at the click location
	 */
	if ((currentMouseAction & VSCEnveloppeViewMouseActionDelete) && movedSinceMouseDown == NO) {
		NSAssert(enveloppePoint.get(), @"Expected enveloppePoint to be non-NULL");
		if (enveloppePoint) {
			_enveloppe->removePoint(enveloppePoint);
			_currentlySelectedPoints.erase(enveloppePoint);
			_pointsInCurrentSelectionRect.erase(enveloppePoint);
		}
	}
	
	/*
	 *	If the mouse action is select or persistant select and the mouse has moved since click down
	 *	then add the points in currentSelectionRect to _currentlySelectedPoints, if non-persistent select then 
	 *	deselect all other points, reset current selection rect.
	 */
    if ((currentMouseAction & (VSCEnveloppeViewMouseActionSelect | VSCEnveloppeViewMouseActionPersistentSelect)) 
		&& movedSinceMouseDown == YES) {
		
		if (currentMouseAction & VSCEnveloppeViewMouseActionSelect) {
			_currentlySelectedPoints.clear();
		}
		
		[self addPointsInRect:currentSelectionRect toPointSet:_currentlySelectedPoints];
		
		_pointsInCurrentSelectionRect.clear();
		
		currentSelectionRect = NSMakeRect(0.0, 0.0, 0.0, 0.0);
		currentSelectionOrigin = NSMakePoint(0.0, 0.0);
		
	}
	
	currentMouseAction = VSCEnveloppeViewMouseActionNone;
	movedSinceMouseDown = NO;
	
	[self setNeedsDisplay:YES];
	
}

-(void) mouseDragged:(NSEvent *)event {
	
	movedSinceMouseDown = YES;
	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:[[self window] contentView]];
	CGFloat deltaX = [event deltaX];
	CGFloat deltaY = [event deltaY];
	
	NSLog(@"Mouse dragged to %@", NSStringFromPoint(locationInView));
	std::cout << _currentlySelectedPoints.size() << " selected point(s)\n";
	
	if (!_enveloppe) {
		currentMouseAction = VSCEnveloppeViewMouseActionNone;
		return;
	}
	
	/*
	 *	Eliminate mouse actions which are impossible once the mouse has mooved (create and delete)
	 */
	currentMouseAction = VSCEnveloppeViewMouseAction(currentMouseAction & ~(VSCEnveloppeViewMouseActionCreate | VSCEnveloppeViewMouseActionDelete));
	
	if (currentMouseAction & (VSCEnveloppeViewMouseActionSelect | VSCEnveloppeViewMouseActionPersistentSelect)) {
		
		NSRect selectionRect = currentSelectionRect;
		CGFloat nx, ny, nw, nh;
		
		if (locationInView.x < currentSelectionOrigin.x) {
			nx = locationInView.x;
			nw = currentSelectionOrigin.x - locationInView.x;
		}
		else {
			nx = currentSelectionOrigin.x;
			nw = locationInView.x - currentSelectionOrigin.x;
		}
		
		if (locationInView.y < currentSelectionOrigin.y) {
			ny = locationInView.y;
			nh = currentSelectionOrigin.y - locationInView.y;
		}
		else {
			ny = currentSelectionOrigin.y;
			nh = locationInView.y - currentSelectionOrigin.y;
		}
		
		currentSelectionRect = NSMakeRect(nx, ny, nw, nh);
		
		[self addPointsInRect:currentSelectionRect toPointSet:_pointsInCurrentSelectionRect];
		
	}
	
	else if (currentMouseAction & VSCEnveloppeViewMouseActionMove) {
		
		currentMouseAction = VSCEnveloppeViewMouseActionMove;
		
		std::set<VSCEnveloppePointPtr>::iterator it;
		
		for (it = _currentlySelectedPoints.begin(); it != _currentlySelectedPoints.end(); it++) {
			NSPoint p = [self pointForEnveloppePoint:(*it)];
			p.x += deltaX;
			p.y += deltaY;
			[self setEnveloppePoint:(*it) withPoint:p];
		}
	}
	
	[self setNeedsDisplay:YES];
	
}

@end
