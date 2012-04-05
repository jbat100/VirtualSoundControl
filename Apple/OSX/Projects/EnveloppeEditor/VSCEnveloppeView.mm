//
//  EnveloppeView.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCEnveloppeView.h"
#import "VSCEnveloppeViewSetup.h"
#import "VSCSound.h"
#import "VSCColour.h"
#import "VSCBoost.h"
#import "VSCException.h"

#import "CGColor+VSCAdditions.h"
#import "NS+VSCGeomOperations.h"

#import <QuartzCore/QuartzCore.h>

#import <cmath>
#import <assert.h>

@interface VSCEnveloppeView ()

@property (nonatomic, assign, readwrite) NSRect currentSelectionRect;
@property (nonatomic, assign, readwrite) NSPoint currentSelectionOrigin;

-(void) purgeCurrentlySelectedPoints;
-(void) addPointsInRect:(NSRect)rect toPointSet:(std::set<VSCEnveloppePointPtr>&)pointSet;

@end



@implementation VSCEnveloppeView

@synthesize mainEnveloppeLayer = _mainEnveloppeLayer;
@synthesize backgroundEnveloppesLayer = _backgroundEnveloppesLayer;
@synthesize dataSource = _dataSource; 


- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _enveloppe = VSCEnveloppePtr();
		_enveloppeViewSetup = VSCEnveloppeEditorGUIConfigPtr(new VSCEnveloppeEditorGUIConfig());
    }
    return self;
}


-(void) reloadEnveloppes {
    
    _enveloppe = VSCEnveloppePtr();
    _backgroundEnveloppeList.clear();
    
    if (self.dataSource) {
        
        _enveloppe = [self.dataSource mainEnveloppeForEnveloppeView:self];
        _backgroundEnveloppeList = [self.dataSource backgroundEnveloppesForEnveloppeView:self];
    }
    
}

-(void) redrawMainEnveloppe {
    [self.mainEnveloppeLayer setNeedsDisplay];
}

-(void) redrawAllEnveloppes {
    [self.backgroundEnveloppesLayer setNeedDisplay];
}



#pragma mark - VSCEnveloppeEditor Methods And Point Selection Management

-(void) purgeCurrentlySelectedPoints {
    
    /*
     *  This checks that all the points in the currently selected sets are still in the enveloppe points 
     *  (points can be removed from the enveloppe after add/displace points)
     */
    
    if (!_enveloppe) {
        _currentlySelectedPoints.clear();
        _pointsInCurrentSelectionRect.clear();
    }
    
    for (VSCEnveloppe::PointSet::iterator setIt = _currentlySelectedPoints.begin(); setIt != _currentlySelectedPoints.end(); setIt++) {
        VSCEnveloppe::ConstPointIterator beginEnvPntIt = _enveloppe->getPointBeginConstIterator();
        VSCEnveloppe::ConstPointIterator endEnvPntIt = _enveloppe->getPointEndConstIterator();
        VSCEnveloppe::ConstPointIterator envPntIt = std::find(beginEnvPntIt, endEnvPntIt, *setIt);
        if (envPntIt == endEnvPntIt) {
            _currentlySelectedPoints.erase(setIt);
        }
    }
    
    for (VSCEnveloppe::PointSet::iterator setIt = _pointsInCurrentSelectionRect.begin(); setIt != _pointsInCurrentSelectionRect.end(); setIt++) {
        VSCEnveloppe::ConstPointIterator beginEnvPntIt = _enveloppe->getPointBeginConstIterator();
        VSCEnveloppe::ConstPointIterator endEnvPntIt = _enveloppe->getPointEndConstIterator();
        VSCEnveloppe::ConstPointIterator envPntIt = std::find(beginEnvPntIt, endEnvPntIt, *setIt);
        if (envPntIt == endEnvPntIt) {
            _pointsInCurrentSelectionRect.erase(setIt);
        }
    }
    
}

-(BOOL) enveloppeIsEditable:(VSCEnveloppePtr)enveloppe {
    
    if (enveloppe == _enveloppe) {
        return YES;
    }
    
    return NO;
    
}

-(BOOL) pointIsSelected:(VSCEnveloppePointPtr)enveloppePoint {
    
    VSCEnveloppe::PointSet::const_iterator pointIt = _pointsInCurrentSelectionRect.find(enveloppePoint);
    
    if (pointIt != _pointsInCurrentSelectionRect.end()) {
        return YES;
    }
    
    return NO;
    
}

-(NSRect) currentSelectionRectForEnveloppe:(VSCEnveloppePtr)enveloppe {
    
    if (enveloppe == _enveloppe) {
        return self.currentSelectionRect;
    }
    
    return NSZeroRect;
    
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
    
    VSCSFloat minTime, maxTime, minValue, maxValue;
    
    try {
        minTime = _enveloppe->minTime();
        maxTime = _enveloppe->maxTime();
        minValue = _enveloppe->minValue();
        maxValue = _enveloppe->maxValue();
        
        VSCSFloat timeMargin = std::abs((maxTime-minTime)*0.2); 
        VSCSFloat valueMargin = std::abs((maxValue-minValue)*0.2); 
        
        //minTime -= timeMargin
        maxTime += timeMargin;
        minValue -= valueMargin;
        maxValue += valueMargin;
    } 
    catch (VSCEnveloppeEmptyException& e) {
        _enveloppeViewSetup->setToDefault();
        return;
    }
    
    if (_enveloppeViewSetup) {
        _enveloppeViewSetup->setTimeRange(VSCEnveloppe::TimeRange(minTime, maxTime-minTime));
        _enveloppeViewSetup->setValueRange(VSCEnveloppe::ValueRange(minTime, maxValue-minValue));
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
	//[self setNeedsDisplay:YES];
    [self.layer setNeedsDisplay];
}

-(VSCEnveloppeEditorGUIConfigPtr) getEnveloppeViewSetup {
    return _enveloppeViewSetup;
}

-(void) setEnveloppeViewSetup:(VSCEnveloppeEditorGUIConfigPtr)enveloppeViewSetup {
	assert(enveloppeViewSetup);
    _enveloppeViewSetup = enveloppeViewSetup;
}

-(VSCEnveloppe::PointSet&)getCurrentlySelectedPoints {
    return _currentlySelectedPoints;
}

-(void)getCurrentlySelectedPoints:(VSCEnveloppe::PointSet&)points {
    for (VSCEnveloppe::PointSet::iterator it = _currentlySelectedPoints.begin(); it != _currentlySelectedPoints.end(); it++) {
		points.insert(*it);
	}
}

-(void)setCurrentlySelectedPoints:(VSCEnveloppe::PointSet&)points {
    _currentlySelectedPoints.clear();
    for (VSCEnveloppe::PointSet::iterator it = points.begin(); it != points.end(); it++) {
		_currentlySelectedPoints.insert(*it);
	}
}


#pragma mark - View to enveloppe conversion tools

-(VSCSFloat) valueDeltaForPointYDelta:(VSCSFloat)pointYDelta {
	VSCSFloat range = _enveloppeViewSetup->getValueRange().second; 
	return (pointYDelta / (VSCSFloat)self.frame.size.height) * range;
}

-(VSCSFloat) timeDeltaForPointXDelta:(VSCSFloat)pointXDelta {
	VSCSFloat range = _enveloppeViewSetup->getTimeRange().second; 
	return (pointXDelta / (VSCSFloat)self.frame.size.width) * range;
}

-(VSCSFloat) valueForPoint:(NSPoint)point {

	assert(_enveloppeViewSetup);
	
	if (!_enveloppeViewSetup)
		return 0.0;
	VSCSFloat normalisedY = (point.y / (VSCSFloat)self.frame.size.height);
	VSCSFloat range = _enveloppeViewSetup->getValueRange().second; 
	VSCSFloat adjustedY = _enveloppeViewSetup->getValueRange().first  + (normalisedY*range);
	
	return adjustedY;
	
}

-(VSCSFloat) timeForPoint:(CGPoint)point {
	
	assert(_enveloppeViewSetup);
	
	if (!_enveloppeViewSetup)
		return 0.0;
	VSCSFloat normalisedX = (point.x / (VSCSFloat)self.frame.size.width);
	VSCSFloat range = _enveloppeViewSetup->getTimeRange().second; 
	return (NSTimeInterval)(_enveloppeViewSetup->getValueRange().first  + (normalisedX*range));
	
}

-(NSPoint) pointForEnveloppePoint:(VSCEnveloppePointPtr)enveloppePoint {
	return [self pointForTime:(NSTimeInterval)enveloppePoint->getTime() value:(VSCSFloat)enveloppePoint->getValue()];
}

-(NSPoint) pointForTime:(NSTimeInterval)time value:(VSCSFloat)value {
	
	VSCSFloat timeRange = _enveloppeViewSetup->getTimeRange().second; 
	VSCSFloat timePerPixel = timeRange / self.frame.size.width; 
	VSCSFloat x = (time - _enveloppeViewSetup->getValueRange().first) / timePerPixel;
	
	VSCSFloat valueRange = _enveloppeViewSetup->getValueRange().second;  
	VSCSFloat valuePerPixel = valueRange / self.frame.size.height; 
	VSCSFloat y = (value - _enveloppeViewSetup->getValueRange().first) / valuePerPixel;
	
	return NSMakePoint(x, y);
}

-(BOOL) point:(NSPoint)p touchesEnveloppePoint:(VSCEnveloppePointPtr)enveloppePoint {
	
	assert(_enveloppeViewSetup);
	
	float radius = _enveloppeViewSetup->getActiveDisplaySetup().getControlPointRadius();
	
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
	
	for (VSCEnveloppe::ConstPointIterator it = _enveloppe->getPointBeginIterator(); it !=_enveloppe->getPointEndIterator(); it++) {
		if ([self point:point touchesEnveloppePoint:(*it)])
			return (*it);
	}
	
	return VSCEnveloppePointPtr();
	
}

-(void) getEnveloppePoints:(VSCEnveloppe::PointList&)ps InRect:(NSRect)rect {
	
	if (!_enveloppe) 
		return;
	
	for (VSCEnveloppe::ConstPointIterator it = _enveloppe->getPointBeginIterator(); it !=_enveloppe->getPointEndIterator(); it++) {
		NSPoint p = [self pointForEnveloppePoint:(*it)];
		if (CGRectContainsPoint(rect, p))
			ps.push_back(*it);
	}
	
}

-(void) setEnveloppePoint:(VSCEnveloppePointPtr)controlPoint withPoint:(NSPoint)p {
    
    // this should NOT be done with points already in the enveloppe (use displacement instead)
    
	controlPoint->setValue([self valueForPoint:p]);
	controlPoint->setTime([self timeForPoint:p]);
}

-(VSCEnveloppePointPtr) createEnveloppePointForPoint:(NSPoint)point {
	
	VSCSFloat t = [self timeForPoint:point];
	
    /*
	if (t<0.0) 
		return VSCEnveloppePointPtr();
	*/
    
	return VSCEnveloppePointPtr(new VSCEnveloppePoint([self valueForPoint:point], t));
	
}

-(void) addPointsInRect:(NSRect)rect toPointSet:(VSCEnveloppe::PointSet&)pointSet {
    VSCEnveloppe::PointList rectPoints; 
	[self getEnveloppePoints:rectPoints InRect:rect];
	for (VSCEnveloppe::PointIterator it = rectPoints.begin(); it != rectPoints.end(); it++) {
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
		
		/*
		 *	If mouse click was not done over an enveloppe point
		 */
		if (!enveloppePoint) {
			
			/*
			 * If there are points selected then a simple click should deselect everything
			 */
			if (_currentlySelectedPoints.size() > 0) {
				_currentlySelectedPoints.clear();
				_pointsInCurrentSelectionRect.clear();
				currentMouseAction = VSCEnveloppeViewMouseActionNone;
			}
			
			else {
				
				currentMouseAction = VSCEnveloppeViewMouseAction(VSCEnveloppeViewMouseActionCreate | 
																 VSCEnveloppeViewMouseActionSelect);
				_currentlySelectedPoints.clear();
				
			}
			
		}
		
		/*
		 *	If mouse click was done over an enveloppe point, the select the point and wait
		 *	wait for further action (mouse up or mouse dragged) to determine whether to delete or move
		 */
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
		NSAssert(!enveloppePoint, @"Expected enveloppePoint to be NULL");
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
			VSCEnveloppe::PointSet::iterator iter = _currentlySelectedPoints.find(enveloppePoint);
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
		NSAssert(enveloppePoint, @"Expected enveloppePoint to be non-NULL");
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
		
		[self addPointsInRect:currentSelectionRect toPointSet:_currentlySelectedPoints];
		
		_pointsInCurrentSelectionRect.clear();
		
		currentSelectionRect = NSMakeRect(0.0, 0.0, 0.0, 0.0);
		currentSelectionOrigin = NSMakePoint(0.0, 0.0);
		
	}
	
	currentMouseAction = VSCEnveloppeViewMouseActionNone;
	movedSinceMouseDown = NO;
    
    [self purgeCurrentlySelectedPoints];
	
	//[self setNeedsDisplay:YES];
    //[self.enveloppeLayer setNeedsDisplay];
    
    [self.layer setNeedsDisplay];
	
}

-(void) mouseDragged:(NSEvent *)event {
	
	movedSinceMouseDown = YES;
	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:[[self window] contentView]];
	CGFloat deltaX = [event deltaX];
	CGFloat deltaY = -[event deltaY];
	
	NSLog(@"Mouse dragged to %@", NSStringFromPoint(locationInView));
	std::cout << _currentlySelectedPoints.size() << " selected point(s)\n";
	
	if (!_enveloppe) {
		currentMouseAction = VSCEnveloppeViewMouseActionNone;
		return;
	}
	
	/*
	 *	Eliminate mouse actions which are impossible once the mouse has moved (create and delete)
	 */
	currentMouseAction = (VSCEnveloppeViewMouseAction)
    (currentMouseAction & ~(VSCEnveloppeViewMouseActionCreate | VSCEnveloppeViewMouseActionDelete));
	
	if (currentMouseAction & (VSCEnveloppeViewMouseActionSelect | VSCEnveloppeViewMouseActionPersistentSelect)) {
		
		//NSRect selectionRect = currentSelectionRect;
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
		
		_pointsInCurrentSelectionRect.clear();
		
		[self addPointsInRect:currentSelectionRect toPointSet:_pointsInCurrentSelectionRect];
		
	}
	
	else if (currentMouseAction & VSCEnveloppeViewMouseActionMove) {
		
		currentMouseAction = VSCEnveloppeViewMouseActionMove;
		
		VSCEnveloppe::PointSet::iterator it;
        
        VSCSFloat valueDelta = [self valueDeltaForPointYDelta:deltaY];
        VSCSFloat timeDelta = [self timeDeltaForPointXDelta:deltaX];
		
        VSCEnveloppe::PointList moveList;
		for (it = _currentlySelectedPoints.begin(); it != _currentlySelectedPoints.end(); it++) {
			//NSPoint p = [self pointForEnveloppePoint:(*it)];
			//p.x += deltaX;
			//p.y += deltaY;
			//[self setEnveloppePoint:(*it) withPoint:p];
            moveList.push_back(*it);
		}
        
        _enveloppe->displacePoints(moveList, timeDelta, valueDelta);
		
	}
    
    [self purgeCurrentlySelectedPoints];
	
	//[self setNeedsDisplay:YES];
    //[self.enveloppeLayer setNeedsDisplay];
	
    [self.layer setNeedsDisplay];
    
}

@end