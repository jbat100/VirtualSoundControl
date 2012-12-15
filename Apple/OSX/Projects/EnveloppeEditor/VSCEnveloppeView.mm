//
//  EnveloppeView.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCEnveloppeView.h"
#import "VSCEnveloppeGUI.h"
#import "VSCSound.h"
#import "VSC::Color.h"
#import "VSCBoost.h"
#import "VSCMath.h"
#import "VSCException.h"

#import "CGColor+VSCAdditions.h"
#import "NS+VSCGeomOperations.h"

#import <QuartzCore/QuartzCore.h>

#import <cmath>
#import <assert.h>

@interface VSC::EnveloppeView ()
{
    
@private
    
    VSC::Enveloppe::SPtr     _enveloppe;
    VSC::Enveloppe::List  _backgroundEnveloppeList;
    
    /*
     *  An editor setup
     */
    VSC::EnveloppeEditorGUIConfig::SPtr _enveloppeEditorGUIConfig;
	
	/*
     *  Keep track of the current grid points and their corresponding pixel
	 *  so that they do not need to be calculated on every draw, need updating
	 *	when frame/bounds change or view range/zoom changes
     */
	
    /*
     *  Keeps track of the currently selected points for group operations (move for example)
     *  A set is more appropriate than a list as we don't care about ordering and adding
     *  without needing to check for presence (sets cannot have duplicates) is an advantage
     */
    VSC::Enveloppe::PointSet _currentlySelectedPoints;
	VSC::Enveloppe::PointSet _pointsInCurrentSelectionRect;
	
    /*
     *  Keeps track of the current mouse action
     */
	VSC::EnveloppeViewMouseAction currentMouseAction;
	BOOL movedSinceMouseDown;
    
}

@property (nonatomic, assign, readwrite) NSRect currentSelectionRect;
@property (nonatomic, assign, readwrite) NSPoint currentSelectionOrigin;

-(void) purgeCurrentlySelectedPoints;
-(void) addPointsInRect:(NSRect)rect toPointSet:(std::set<VSC::EnveloppePoint::SPtr>&)pointSet;

@end



@implementation VSC::EnveloppeView

@synthesize mainEnveloppeLayer = _mainEnveloppeLayer;
@synthesize backgroundEnveloppesLayer = _backgroundEnveloppesLayer;
@synthesize dataSource = _dataSource; 

@synthesize currentSelectionRect = _currentSelectionRect;
@synthesize currentSelectionOrigin = _currentSelectionOrigin;

#pragma mark - NSView Methods

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _enveloppe = VSC::Enveloppe::SPtr();
		_enveloppeEditorGUIConfig = VSC::EnveloppeEditorGUIConfig::SPtr(new VSC::EnveloppeEditorGUIConfig());
    }
    return self;
}

-(void)setFrameSize:(NSSize)newSize {
    _enveloppeEditorGUIConfig->setEditorSize(VSC::MakeSizeFromSize(newSize));
    [super setFrameSize:newSize];
}

#pragma mark - Custom Draw Methods

-(void) reloadEnveloppes {
    
    _enveloppe = VSC::Enveloppe::SPtr();
    _backgroundEnveloppeList.clear();
    
    if (self.dataSource) {
        
        _enveloppe = [self.dataSource mainEnveloppeForEnveloppeView:self];
        _backgroundEnveloppeList = [self.dataSource backgroundEnveloppesForEnveloppeView:self];
        
        [self.mainEnveloppeLayer addEnveloppe:_enveloppe];
    }
    
}

-(void) redrawMainEnveloppe {
    [self.mainEnveloppeLayer setNeedsDisplay];
}

-(void) redrawAllEnveloppes {
    [self.backgroundEnveloppesLayer setNeedsDisplay];
}



#pragma mark - VSC::EnveloppeEditor Methods And Point Selection Management

-(void) purgeCurrentlySelectedPoints {
    
    /*
     *  This checks that all the points in the currently selected sets are still in the enveloppe points 
     *  (points can be removed from the enveloppe after add/displace points)
     */
    
    if (!_enveloppe) {
        _currentlySelectedPoints.clear();
        _pointsInCurrentSelectionRect.clear();
    }
    
    for (VSC::Enveloppe::PointSet::iterator setIt = _currentlySelectedPoints.begin(); setIt != _currentlySelectedPoints.end(); setIt++) {
        VSC::Enveloppe::ConstPointIterator beginEnvPntIt = _enveloppe->getPointBeginConstIterator();
        VSC::Enveloppe::ConstPointIterator endEnvPntIt = _enveloppe->getPointEndConstIterator();
        VSC::Enveloppe::ConstPointIterator envPntIt = std::find(beginEnvPntIt, endEnvPntIt, *setIt);
        if (envPntIt == endEnvPntIt) {
            _currentlySelectedPoints.erase(setIt);
        }
    }
    
    for (VSC::Enveloppe::PointSet::iterator setIt = _pointsInCurrentSelectionRect.begin(); setIt != _pointsInCurrentSelectionRect.end(); setIt++) {
        VSC::Enveloppe::ConstPointIterator beginEnvPntIt = _enveloppe->getPointBeginConstIterator();
        VSC::Enveloppe::ConstPointIterator endEnvPntIt = _enveloppe->getPointEndConstIterator();
        VSC::Enveloppe::ConstPointIterator envPntIt = std::find(beginEnvPntIt, endEnvPntIt, *setIt);
        if (envPntIt == endEnvPntIt) {
            _pointsInCurrentSelectionRect.erase(setIt);
        }
    }
    
}

-(BOOL) enveloppeIsEditable:(VSC::Enveloppe::SPtr)enveloppe {
    
    if (enveloppe == _enveloppe) {
        return YES;
    }
    
    return NO;
    
}

-(BOOL) pointIsSelected:(VSC::EnveloppePoint::SPtr)enveloppePoint {
    
    VSC::Enveloppe::PointSet::const_iterator pointIt = _pointsInCurrentSelectionRect.find(enveloppePoint);
    
    if (pointIt != _pointsInCurrentSelectionRect.end()) {
        return YES;
    }
    
    return NO;
    
}

-(NSRect) currentSelectionRectForEnveloppe:(VSC::Enveloppe::SPtr)enveloppe {
    
    if (enveloppe == _enveloppe) {
        return self.currentSelectionRect;
    }
    
    return NSZeroRect;
    
}

-(VSC::EnveloppeEditorGUIConfig::SPtr) enveloppeEditorGUIConfig {
    return _enveloppeEditorGUIConfig;
}

#pragma mark - Auto Adjust View Setup

/* 
 * Automatic View Setup
 */
-(void)autoAdjustEnveloppeViewSetup {
    
    assert(_enveloppeEditorGUIConfig);
	
	if (!_enveloppe) {
		_enveloppeEditorGUIConfig->setToDefault();
	}
    
    Float minTime, maxTime, minValue, maxValue;
    
    try {
        minTime = _enveloppe->minTime();
        maxTime = _enveloppe->maxTime();
        minValue = _enveloppe->minValue();
        maxValue = _enveloppe->maxValue();
        
        Float timeMargin = std::abs((maxTime-minTime)*0.2); 
        Float valueMargin = std::abs((maxValue-minValue)*0.2); 
        
        //minTime -= timeMargin
        maxTime += timeMargin;
        minValue -= valueMargin;
        maxValue += valueMargin;
    } 
    catch (VSC::EnveloppeEmptyException& e) {
        _enveloppeEditorGUIConfig->setToDefault();
        return;
    }
    
    if (_enveloppeEditorGUIConfig) {
        _enveloppeEditorGUIConfig->setTimeRange(VSC::Enveloppe::TimeRange(minTime, maxTime-minTime));
        _enveloppeEditorGUIConfig->setValueRange(VSC::Enveloppe::ValueRange(minTime, maxValue-minValue));
    }
    
}

#pragma mark - C++ Setters/Getters

-(VSC::Enveloppe::SPtr) getEnveloppe {
    return _enveloppe;
}

-(void) setEnveloppe:(VSC::Enveloppe::SPtr)enveloppe {
	_currentlySelectedPoints.clear();
	_pointsInCurrentSelectionRect.clear();
    _enveloppe = enveloppe;
	//[self setNeedsDisplay:YES];
    [self.layer setNeedsDisplay];
}

-(VSC::EnveloppeEditorGUIConfig::SPtr) getEnveloppeViewSetup {
    return _enveloppeEditorGUIConfig;
}

-(void) setEnveloppeViewSetup:(VSC::EnveloppeEditorGUIConfig::SPtr)enveloppeViewSetup {
	assert(enveloppeViewSetup);
    _enveloppeEditorGUIConfig = enveloppeViewSetup;
}

-(VSC::Enveloppe::PointSet&)getCurrentlySelectedPoints {
    return _currentlySelectedPoints;
}

-(void)getCurrentlySelectedPoints:(VSC::Enveloppe::PointSet&)points {
    for (VSC::Enveloppe::PointSet::iterator it = _currentlySelectedPoints.begin(); it != _currentlySelectedPoints.end(); it++) {
		points.insert(*it);
	}
}

-(void)setCurrentlySelectedPoints:(VSC::Enveloppe::PointSet&)points {
    _currentlySelectedPoints.clear();
    for (VSC::Enveloppe::PointSet::iterator it = points.begin(); it != points.end(); it++) {
		_currentlySelectedPoints.insert(*it);
	}
}


#pragma mark - View to enveloppe conversion tools

-(NSPoint) pointForEnveloppePoint:(VSC::EnveloppePoint::SPtr)enveloppePoint {
	return [self pointForTime:(NSTimeInterval)enveloppePoint->getTime() value:(Float)enveloppePoint->getValue()];
}

-(NSPoint) pointForTime:(NSTimeInterval)time value:(Float)value {
	CGFloat x = _enveloppeEditorGUIConfig->pointForTime(time);
	CGFloat y = _enveloppeEditorGUIConfig->pointForValue(value);
	return NSMakePoint(x, y);
}

-(BOOL) point:(const NSPoint)p touchesEnveloppePoint:(VSC::EnveloppePoint::SPtr)enveloppePoint {
	
	assert(_enveloppeEditorGUIConfig);
	
	float radius = _enveloppeEditorGUIConfig->getPointSelectionRadius();
	
	/* First do a simple sqaure test to discard faster */
	const NSPoint envP = [self pointForEnveloppePoint:enveloppePoint];
	CGRect testRect = CGRectMake(envP.x-radius, envP.y, radius*2, radius*2);
	if (!CGRectContainsPoint(testRect, envP))
		return NO;
	
	/* If first test succeeded, do proper test (template function can't work out the return type so specify it...) */
	CGFloat distanceToPoint = VSC::Geom::DistanceBetweenPoints<CGFloat>(p, envP);
	if (distanceToPoint < radius) 
		return YES;
	
	return NO;
	
}

#pragma mark - Enveloppe points handling

-(VSC::EnveloppePoint::SPtr) enveloppePointUnderPoint:(NSPoint)point {
	if (!_enveloppe) {
		return VSC::EnveloppePoint::SPtr();
	}
	for (VSC::Enveloppe::ConstPointIterator it = _enveloppe->getPointBeginIterator(); it !=_enveloppe->getPointEndIterator(); it++) {
		if ([self point:point touchesEnveloppePoint:(*it)])
			return (*it);
	}
	return VSC::EnveloppePoint::SPtr();
}

-(void) getEnveloppePoints:(VSC::Enveloppe::PointList&)ps InRect:(NSRect)rect {
	if (!_enveloppe) 
		return;
	for (VSC::Enveloppe::ConstPointIterator it = _enveloppe->getPointBeginIterator(); it !=_enveloppe->getPointEndIterator(); it++) {
		NSPoint p = [self pointForEnveloppePoint:(*it)];
		if (CGRectContainsPoint(rect, p))
			ps.push_back(*it);
	}
}

-(void) addPointsInRect:(NSRect)rect toPointSet:(VSC::Enveloppe::PointSet&)pointSet {
    VSC::Enveloppe::PointList rectPoints; 
	[self getEnveloppePoints:rectPoints InRect:rect];
	for (VSC::Enveloppe::PointIterator it = rectPoints.begin(); it != rectPoints.end(); it++) {
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
		currentMouseAction = VSC::EnveloppeViewMouseActionNone;
		return;
	}
	
	VSC::EnveloppePoint::SPtr enveloppePoint = [self enveloppePointUnderPoint:locationInView];
	
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
		currentMouseAction = VSC::EnveloppeViewMouseActionPersistentSelect;
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
				currentMouseAction = VSC::EnveloppeViewMouseActionNone;
			}
			
			else {
				
				currentMouseAction = VSC::EnveloppeViewMouseAction(VSC::EnveloppeViewMouseActionCreate | 
																 VSC::EnveloppeViewMouseActionSelect);
				_currentlySelectedPoints.clear();
				
			}
			
		}
		
		/*
		 *	If mouse click was done over an enveloppe point, the select the point and wait
		 *	wait for further action (mouse up or mouse dragged) to determine whether to delete or move
		 */
		else {
			currentMouseAction = VSC::EnveloppeViewMouseAction(VSC::EnveloppeViewMouseActionDelete | 
															 VSC::EnveloppeViewMouseActionMove);
			_currentlySelectedPoints.insert(enveloppePoint);
		}
		
	}
		
	if (currentMouseAction & (VSC::EnveloppeViewMouseActionSelect | VSC::EnveloppeViewMouseActionPersistentSelect)) {
		self.currentSelectionRect = NSMakeRect(locationInView.x, locationInView.y, 0.0, 0.0);
		self.currentSelectionOrigin = NSMakePoint(locationInView.x, locationInView.y);
	}
	
    [self setNeedsDisplay:YES];
}

-(void) mouseUp:(NSEvent *)event {
	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:[[self window] contentView]];
    [self setNeedsDisplay:YES];
	NSLog(@"Mouse up at x: %f, y: %f", locationInView.x, locationInView.y);
	
	if (!_enveloppe) {
		currentMouseAction = VSC::EnveloppeViewMouseActionNone;
		return;
    }
	
    VSC::EnveloppePoint::SPtr enveloppePoint = [self enveloppePointUnderPoint:locationInView];
    
	/*
	 *	If the mouse action contains create and the mouse has not moved since click down, then create
	 *	a new point at the click location
	 */
	if ((currentMouseAction & VSC::EnveloppeViewMouseActionCreate) && movedSinceMouseDown == NO) {
		NSAssert(!enveloppePoint, @"Expected enveloppePoint to be NULL");
        VSC::EnveloppeCoordinate::SPtr coord = _enveloppeEditorGUIConfig->createEnveloppeCoordinateForPoint(VSC::MakePointFromPoint(locationInView));
        VSC::EnveloppePoint::SPtr newPoint = VSC::EnveloppePoint::SPtr(new VSC::EnveloppePoint(*coord));
		_enveloppe->addPoint(newPoint);
	}
	
	/*
	 *	If the mouse action is select or persistant select and the mouse has not moved since click down
	 *	then select/deselect the point at mouse location, if non-persistent select then deselect all other 
	 *	points, reset current selection rect
	 */
	if ((currentMouseAction & (VSC::EnveloppeViewMouseActionSelect | VSC::EnveloppeViewMouseActionPersistentSelect)) && 
		movedSinceMouseDown == NO) {
		
		if (currentMouseAction & VSC::EnveloppeViewMouseActionSelect) {
			_currentlySelectedPoints.clear();
		}
		
		// If a point exists at the point where the click occured then select/deselect the point
		if (enveloppePoint) {
			VSC::Enveloppe::PointSet::iterator iter = _currentlySelectedPoints.find(enveloppePoint);
			// if the point is not selected, add it to selected...
			if (iter == _currentlySelectedPoints.end()) {
				_currentlySelectedPoints.insert(enveloppePoint);
			}
			// else remove it from the currently selected points
			else {
				_currentlySelectedPoints.erase(enveloppePoint);
			}
		}
		
		self.currentSelectionRect = NSMakeRect(0.0, 0.0, 0.0, 0.0);
		self.currentSelectionOrigin = NSMakePoint(0.0, 0.0);

	}

	/*
	 *	If the mouse action contains delete and the mouse has not moved since click down, then delete
	 *	the point at the click location
	 */
	if ((currentMouseAction & VSC::EnveloppeViewMouseActionDelete) && movedSinceMouseDown == NO) {
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
    if ((currentMouseAction & (VSC::EnveloppeViewMouseActionSelect | VSC::EnveloppeViewMouseActionPersistentSelect)) 
		&& movedSinceMouseDown == YES) {
		
		[self addPointsInRect:self.currentSelectionRect toPointSet:_currentlySelectedPoints];
		
		_pointsInCurrentSelectionRect.clear();
		
		self.currentSelectionRect = NSMakeRect(0.0, 0.0, 0.0, 0.0);
		self.currentSelectionOrigin = NSMakePoint(0.0, 0.0);
		
	}
	
	currentMouseAction = VSC::EnveloppeViewMouseActionNone;
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
		currentMouseAction = VSC::EnveloppeViewMouseActionNone;
		return;
	}
	
	/*
	 *	Eliminate mouse actions which are impossible once the mouse has moved (create and delete)
	 */
	currentMouseAction = (VSC::EnveloppeViewMouseAction)
    (currentMouseAction & ~(VSC::EnveloppeViewMouseActionCreate | VSC::EnveloppeViewMouseActionDelete));
	
	if (currentMouseAction & (VSC::EnveloppeViewMouseActionSelect | VSC::EnveloppeViewMouseActionPersistentSelect)) {
		
		//NSRect selectionRect = currentSelectionRect;
		CGFloat nx, ny, nw, nh;
		
		if (locationInView.x < self.currentSelectionOrigin.x) {
			nx = locationInView.x;
			nw = self.currentSelectionOrigin.x - locationInView.x;
		}
		else {
			nx = self.currentSelectionOrigin.x;
			nw = locationInView.x - self.currentSelectionOrigin.x;
		}
		
		if (locationInView.y < self.currentSelectionOrigin.y) {
			ny = locationInView.y;
			nh = self.currentSelectionOrigin.y - locationInView.y;
		}
		else {
			ny = self.currentSelectionOrigin.y;
			nh = locationInView.y - self.currentSelectionOrigin.y;
		}
		
		self.currentSelectionRect = NSMakeRect(nx, ny, nw, nh);
		
		_pointsInCurrentSelectionRect.clear();
		
		[self addPointsInRect:self.currentSelectionRect toPointSet:_pointsInCurrentSelectionRect];
		
	}
	
	else if (currentMouseAction & VSC::EnveloppeViewMouseActionMove) {
		
		currentMouseAction = VSC::EnveloppeViewMouseActionMove;
		
		VSC::Enveloppe::PointSet::iterator it;
        
        Float valueDelta = _enveloppeEditorGUIConfig->valueDeltaForPointDelta(deltaY);
        Float timeDelta = _enveloppeEditorGUIConfig->timeDeltaForPointDelta(deltaX);
		
        VSC::Enveloppe::PointList moveList;
		for (it = _currentlySelectedPoints.begin(); it != _currentlySelectedPoints.end(); it++) {
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
