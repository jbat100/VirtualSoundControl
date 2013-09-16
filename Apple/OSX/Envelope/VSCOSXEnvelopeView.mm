//
//  EnvelopeView.m
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCOSXEnvelopeView.h"
#import "VSCEnvelopeGUI.h"
#import "VSCSound.h"
#import "VSCColor.h"
#import "VSCBoost.h"
#import "VSCMath.h"
#import "VSCException.h"

#import "CGColor+VSCAdditions.h"
#import "NS+VSCGeomOperations.h"

#import <QuartzCore/QuartzCore.h>

#include <cmath>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

using namespace VSC;

/*
 * We don't want to call [self setWantsLayer:YES]; BEFORE setLayer as we want a layer hosting view, not a layer backed view
 
 APPLE DOC:
 
 A layer-backed view is a view that is backed by a Core Animation layer.
 Any drawing done by the view is cached in the backing layer. You configure a layer-backed view by invoking setWantsLayer:
 with a value of YES. The view class automatically creates a backing layer for you (using makeBackingLayer if overridden), and
 you must use the view class’s drawing mechanisms. When using layer-backed views you should never interact directly with the layer.
 Instead you must use the standard view programming practices.
 
 */

//[self setWantsLayer:YES]; // view's backing store is using a Core Animation Layer

//[mainEnveloppeView setLayer:viewLayer];

/*
 * We DO want to call [self setWantsLayer:YES]; AFTER setLayer
 *
 
 APPLE DOC:
 
 A layer-hosting view is a view that contains a Core Animation layer that you intend to manipulate directly.
 You create a layer-hosting view by instantiating a Core Animation layer class and supplying that layer to the
 view’s setLayer: method. After doing so, you then invoke setWantsLayer: with a value of YES. This method order
 is crucial. When using a layer-hosting view you should not rely on the view for drawing, nor should you add
 subviews to the layer-hosting view. The root layer (the layer set using setLayer:) should be treated as the
 root layer of the layer tree and you should only use Core Animation drawing and animation methods. You still
 use the view for handling mouse and keyboard events, but any resulting drawing must be handled by Core Animation.
 
 */

@interface VSCOSXEnvelopeView ()
{
    /*
     *  Keeps track of the currently selected points for group operations (move for example)
     *  A set is more appropriate than a list as we don't care about ordering and adding
     *  without needing to check for presence (sets cannot have duplicates) is an advantage
     */
    EnvelopePointSet _currentlySelectedPoints;
	EnvelopePointSet _pointsInCurrentSelectionRect;
    
}

@property (nonatomic, assign) VSCOSXEnvelopeViewMouseAction currentMouseAction;
@property (nonatomic, assign) BOOL movedSinceMouseDown;

@property (nonatomic, assign, readwrite) NSRect currentSelectionRect;
@property (nonatomic, assign, readwrite) NSPoint currentSelectionOrigin;

-(void) purgeCurrentlySelectedPoints;
-(void) addPointsInRect:(NSRect)rect toEnvelopePointSet:(std::set<EnvelopePoint::SPtr>&)pointSet;

@end



@implementation VSCOSXEnvelopeView


#pragma mark - NSView Methods

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
		_envelopeEditorGUIConfig = EnvelopeEditorGUIConfig_SPtr(new EnvelopeEditorGUIConfig());
    }
    return self;
}

-(void)setFrameSize:(NSSize)newSize
{
    _envelopeEditorGUIConfig->setEditorSize(MakeSizeFromSize(newSize));
    [super setFrameSize:newSize];
}

#pragma mark - Custom Draw Methods

-(void) redrawMainEnvelope
{
    [self.mainEnvelopeLayer setNeedsDisplay];
}

-(void) redrawAllEnvelopes
{
    [self.mainEnvelopeLayer setNeedsDisplay];
    [self.backgroundEnvelopesLayer setNeedsDisplay];
}



#pragma mark - EnvelopeEditor Methods And Point Selection Management

-(void) purgeCurrentlySelectedPoints
{
    /*
     *  This checks that all the points in the currently selected sets are still in the envelope points 
     *  (points can be removed from the envelope after add/displace points)
     */
    
    Envelope_SPtr envelope = [self.controller currentEnvelope];
    
    if (!envelope)
    {
        _currentlySelectedPoints.clear();
        _pointsInCurrentSelectionRect.clear();
    }
    
    for (EnvelopePointSet::iterator setIt = _currentlySelectedPoints.begin(); setIt != _currentlySelectedPoints.end(); setIt++)
    {
        EnvelopePoints::const_iterator envPntIt = std::find(envelope->getPoints().begin(), envelope->getPoints().end(), *setIt);
        if (envPntIt == envelope->getPoints().end())
        {
            _currentlySelectedPoints.erase(setIt);
        }
    }
    
    for (EnvelopePointSet::iterator setIt = _pointsInCurrentSelectionRect.begin(); setIt != _pointsInCurrentSelectionRect.end(); setIt++)
    {
        EnvelopePoints::const_iterator envPntIt = std::find(envelope->getPoints().begin(), envelope->getPoints().end(), *setIt);
        if (envPntIt == envelope->getPoints().end())
        {
            _pointsInCurrentSelectionRect.erase(setIt);
        }
    }
}

-(BOOL) envelopeIsEditable:(Envelope::SPtr)envelope
{
    if (envelope && envelope == [self.controller currentEnvelope])
    {
        return YES;
    }
    return NO;
}

-(BOOL) selectedEnvelopePointsForEnvelope:(EnvelopePoint::SPtr)envelopePoint
{
    EnvelopePointSet::const_iterator pointIt = _pointsInCurrentSelectionRect.find(envelopePoint);
    if (pointIt != _pointsInCurrentSelectionRect.end())
    {
        return YES;
    }
    return NO;
}

-(NSRect) currentSelectionRectForEnvelope:(Envelope::SPtr)envelope
{
    if (envelope && envelope == [self.controller currentEnvelope])
    {
        return self.currentSelectionRect;
    }
    return NSZeroRect;
}

-(EnvelopeEditorGUIConfig_SPtr) envelopeEditorGUIConfig
{
    return _envelopeEditorGUIConfig;
}

#pragma mark - Auto Adjust View Setup

/* 
 * Automatic View Setup
 */

-(void)autoAdjustEnvelopeViewSetup
{    
    BOOST_ASSERT(_envelopeEditorGUIConfig);
    
    Envelope_SPtr envelope = [self.controller currentEnvelope];
	
	if (!envelope)
    {
		_envelopeEditorGUIConfig->setToDefault();
	}
    
    Float minTime, maxTime, minValue, maxValue;
    
    try
    {
        minTime = envelope->minTime();
        maxTime = envelope->maxTime();
        minValue = envelope->minValue();
        maxValue = envelope->maxValue();
        
        Float timeMargin = std::abs((maxTime-minTime)*0.2); 
        Float valueMargin = std::abs((maxValue-minValue)*0.2); 
        
        //minTime -= timeMargin
        maxTime += timeMargin;
        minValue -= valueMargin;
        maxValue += valueMargin;
    } 
    catch (VSCEnvelopeEmptyException& e)
    {
        _envelopeEditorGUIConfig->setToDefault();
        return;
    }
    
    if (_envelopeEditorGUIConfig)
    {
        _envelopeEditorGUIConfig->setTimeRange(Envelope::TimeRange(minTime, maxTime-minTime));
        _envelopeEditorGUIConfig->setValueRange(Envelope::ValueRange(minTime, maxValue-minValue));
    }
    
}

#pragma mark - C++ Setters/Getters


-(EnvelopeEditorGUIConfig_SPtr) getEnvelopeViewSetup
{
    return _envelopeEditorGUIConfig;
}

-(void) setEnvelopeViewSetup:(EnvelopeEditorGUIConfig_SPtr)envelopeViewSetup
{
	BOOST_ASSERT(envelopeViewSetup);
    _envelopeEditorGUIConfig = envelopeViewSetup;
}

-(EnvelopePointSet&)getCurrentlySelectedPoints
{
    return _currentlySelectedPoints;
}

-(void)getCurrentlySelectedPoints:(EnvelopePointSet&)points
{
    for (EnvelopePointSet::iterator it = _currentlySelectedPoints.begin(); it != _currentlySelectedPoints.end(); it++)
    {
		points.insert(*it);
	}
}

-(void)setCurrentlySelectedPoints:(EnvelopePointSet&)points
{
    _currentlySelectedPoints.clear();
    for (EnvelopePointSet::iterator it = points.begin(); it != points.end(); it++)
    {
		_currentlySelectedPoints.insert(*it);
	}
}


#pragma mark - View to envelope conversion tools

-(NSPoint) pointForEnvelopePoint:(EnvelopePoint::SPtr)envelopePoint
{
	return [self pointForTime:(NSTimeInterval)envelopePoint->getTime() value:(Float)envelopePoint->getValue()];
}

-(NSPoint) pointForTime:(NSTimeInterval)time value:(Float)value
{
	CGFloat x = _envelopeEditorGUIConfig->pointForTime(time);
	CGFloat y = _envelopeEditorGUIConfig->pointForValue(value);
	return NSMakePoint(x, y);
}

-(BOOL) point:(const NSPoint)p touchesEnvelopePoint:(EnvelopePoint::SPtr)envelopePoint
{
	BOOST_ASSERT(_envelopeEditorGUIConfig);
	
	float radius = _envelopeEditorGUIConfig->getPointSelectionRadius();
	
	/* First do a simple sqaure test to discard faster */
	const NSPoint envP = [self pointForEnvelopePoint:envelopePoint];
	CGRect testRect = CGRectMake(envP.x-radius, envP.y, radius*2, radius*2);
    
	if (!CGRectContainsPoint(testRect, envP)) return NO;
	
	/* If first test succeeded, do proper test (template function can't work out the return type so specify it...) */
	CGFloat distanceToPoint = Geom::DistanceBetweenPoints<CGFloat>(p, envP);
	
    if (distanceToPoint < radius) return YES;
	
	return NO;
	
}

#pragma mark - Envelope points handling

-(EnvelopePoint_SPtr) envelopePointUnderPoint:(NSPoint)p
{
    Envelope_SPtr envelope = [self.controller currentEnvelope];
    
	if (!envelope) return EnvelopePoint::SPtr();
	
	BOOST_FOREACH (EnvelopePoint_SPtr point, envelope->getPoints())
    {
		if ([self point:p touchesEnvelopePoint:point]) return point;
	}
	return EnvelopePoint_SPtr();
}

-(void) getEnvelopePoints:(EnvelopePoints&)ps InRect:(NSRect)rect
{
    Envelope_SPtr envelope = [self.controller currentEnvelope];
    
	if (!envelope) return;
    
	BOOST_FOREACH (EnvelopePoint_SPtr point, envelope->getPoints())
    {
		NSPoint p = [self pointForEnvelopePoint:point];
        if (CGRectContainsPoint(rect, p)) ps.push_back(point);
	}
}

-(void) addPointsInRect:(NSRect)rect toEnvelopePointSet:(EnvelopePointSet&)pointSet
{
    EnvelopePoints rectPoints; 
	[self getEnvelopePoints:rectPoints InRect:rect];
    
    BOOST_FOREACH (EnvelopePoint_SPtr point, rectPoints)
    {
		pointSet.insert(point);
	}
}


#pragma mark - Mouse Input

-(BOOL) acceptsFirstMouse:(NSEvent*)event
{
	return YES;
}

-(void) mouseDown:(NSEvent*)event
{	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:[[self window] contentView]];
    [self setNeedsDisplay:YES];
    
    _movedSinceMouseDown = NO;
	
	NSLog(@"Mouse down at x: %f, y: %f", locationInView.x, locationInView.y);
    
    Envelope_SPtr envelope = [self.controller currentEnvelope];
	
	if (!envelope)
    {
		_currentMouseAction = VSCOSXEnvelopeViewMouseActionNone;
		return;
	}
	
	EnvelopePoint::SPtr envelopePoint = [self envelopePointUnderPoint:locationInView];
	
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
	
	if (modifierFlags & NSShiftKeyMask)
    {
		NSLog(@"With shift");
		foundModifier = YES;
		_currentMouseAction = VSCOSXEnvelopeViewMouseActionPersistentSelect;
	}
	else if (modifierFlags & NSAlphaShiftKeyMask)
    {
		NSLog(@"With alpha shift");
		foundModifier = YES;
	}
	else if (modifierFlags & NSControlKeyMask)
    {
		NSLog(@"With control");
		foundModifier = YES;
	}
	else if (modifierFlags & NSAlternateKeyMask)
    {
		NSLog(@"With alternate");
		foundModifier = YES;
	}
	else if (modifierFlags & NSCommandKeyMask)
    {
		NSLog(@"With command");
		foundModifier = YES;
	}
	else
    {
		/*
		 *	If mouse click was not done over an envelope point
		 */
		if (!envelopePoint)
        {
			/*
			 * If there are points selected then a simple click should deselect everything
			 */
			if (_currentlySelectedPoints.size() > 0)
            {
				_currentlySelectedPoints.clear();
				_pointsInCurrentSelectionRect.clear();
				_currentMouseAction = VSCOSXEnvelopeViewMouseActionNone;
			}
			else
            {
				_currentMouseAction = VSCOSXEnvelopeViewMouseAction(VSCOSXEnvelopeViewMouseActionCreate |
                                                                    VSCOSXEnvelopeViewMouseActionSelect);
				_currentlySelectedPoints.clear();
			}
		}
		
		/*
		 *	If mouse click was done over an envelope point, the select the point and wait
		 *	wait for further action (mouse up or mouse dragged) to determine whether to delete or move
		 */
		else
        {
			_currentMouseAction = VSCOSXEnvelopeViewMouseAction(VSCOSXEnvelopeViewMouseActionDelete |
                                                                VSCOSXEnvelopeViewMouseActionMove);
			_currentlySelectedPoints.insert(envelopePoint);
		}
		
	}
		
	if (_currentMouseAction & (VSCOSXEnvelopeViewMouseActionSelect | VSCOSXEnvelopeViewMouseActionPersistentSelect))
    {
		self.currentSelectionRect = NSMakeRect(locationInView.x, locationInView.y, 0.0, 0.0);
		self.currentSelectionOrigin = NSMakePoint(locationInView.x, locationInView.y);
	}
	
    [self setNeedsDisplay:YES];
}

-(void) mouseUp:(NSEvent *)event
{
    Envelope_SPtr envelope = [self.controller currentEnvelope];
    
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:[[self window] contentView]];
    [self setNeedsDisplay:YES];
	NSLog(@"Mouse up at x: %f, y: %f", locationInView.x, locationInView.y);
	
	if (!envelope)
    {
		_currentMouseAction = VSCOSXEnvelopeViewMouseActionNone;
		return;
    }
	
    EnvelopePoint::SPtr envelopePoint = [self envelopePointUnderPoint:locationInView];
    
	/*
	 *	If the mouse action contains create and the mouse has not moved since click down, then create
	 *	a new point at the click location
	 */
    
	if ((_currentMouseAction & VSCOSXEnvelopeViewMouseActionCreate) && _movedSinceMouseDown == NO)
    {
		NSAssert(!envelopePoint, @"Expected envelopePoint to be NULL");
        EnvelopeCoordinate_SPtr coord = _envelopeEditorGUIConfig->createEnvelopeCoordinateForPoint(MakePointFromPoint(locationInView));
        EnvelopePoint::SPtr newPoint = EnvelopePoint::SPtr(new EnvelopePoint(*coord));
		envelope->addPoint(newPoint);
	}
	
	/*
	 *	If the mouse action is select or persistant select and the mouse has not moved since click down
	 *	then select/deselect the point at mouse location, if non-persistent select then deselect all other 
	 *	points, reset current selection rect
	 */
    
	if ((_currentMouseAction & (VSCOSXEnvelopeViewMouseActionSelect | VSCOSXEnvelopeViewMouseActionPersistentSelect)) &&
        _movedSinceMouseDown == NO)
    {
		if (_currentMouseAction & VSCOSXEnvelopeViewMouseActionSelect)
        {
			_currentlySelectedPoints.clear();
		}
		
		// If a point exists at the point where the click occured then select/deselect the point
		if (envelopePoint)
        {
			EnvelopePointSet::iterator iter = _currentlySelectedPoints.find(envelopePoint);
			// if the point is not selected, add it to selected...
			if (iter == _currentlySelectedPoints.end())
            {
				_currentlySelectedPoints.insert(envelopePoint);
			}
			// else remove it from the currently selected points
			else
            {
				_currentlySelectedPoints.erase(envelopePoint);
			}
		}
		
		self.currentSelectionRect = NSMakeRect(0.0, 0.0, 0.0, 0.0);
		self.currentSelectionOrigin = NSMakePoint(0.0, 0.0);
	}

	/*
	 *	If the mouse action contains delete and the mouse has not moved since click down, then delete
	 *	the point at the click location
	 */
    
	if ((_currentMouseAction & VSCOSXEnvelopeViewMouseActionDelete) && _movedSinceMouseDown == NO)
    {
		NSAssert(envelopePoint, @"Expected envelopePoint to be non-NULL");
		if (envelopePoint)
        {
			envelope->removePoint(envelopePoint);
			_currentlySelectedPoints.erase(envelopePoint);
			_pointsInCurrentSelectionRect.erase(envelopePoint);
		}
	}
	
	/*
	 *	If the mouse action is select or persistant select and the mouse has moved since click down
	 *	then add the points in currentSelectionRect to _currentlySelectedPoints, if non-persistent select then 
	 *	deselect all other points, reset current selection rect.
	 */
    
    if ((_currentMouseAction & (VSCOSXEnvelopeViewMouseActionSelect | VSCOSXEnvelopeViewMouseActionPersistentSelect)) &&
        _movedSinceMouseDown == YES)
    {
		[self addPointsInRect:self.currentSelectionRect toEnvelopePointSet:_currentlySelectedPoints];
		
		_pointsInCurrentSelectionRect.clear();
		
		self.currentSelectionRect = NSMakeRect(0.0, 0.0, 0.0, 0.0);
		self.currentSelectionOrigin = NSMakePoint(0.0, 0.0);
	}
	
	_currentMouseAction = VSCOSXEnvelopeViewMouseActionNone;
	_movedSinceMouseDown = NO;
    
    [self purgeCurrentlySelectedPoints];
    
    [self.layer setNeedsDisplay];
}

-(void) mouseDragged:(NSEvent *)event
{
    Envelope_SPtr envelope = [self.controller currentEnvelope];
    
	_movedSinceMouseDown = YES;
	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:[[self window] contentView]];
	CGFloat deltaX = [event deltaX];
	CGFloat deltaY = -[event deltaY];
	
	NSLog(@"Mouse dragged to %@", NSStringFromPoint(locationInView));
	std::cout << _currentlySelectedPoints.size() << " selected point(s)\n";
	
	if (!envelope)
    {
		_currentMouseAction = VSCOSXEnvelopeViewMouseActionNone;
		return;
	}
	
	/*
	 *	Eliminate mouse actions which are impossible once the mouse has moved (create and delete)
	 */
    
	_currentMouseAction = (VSCOSXEnvelopeViewMouseAction) (_currentMouseAction & ~(VSCOSXEnvelopeViewMouseActionCreate |
                                                                                   VSCOSXEnvelopeViewMouseActionDelete));
	
	if (_currentMouseAction & (VSCOSXEnvelopeViewMouseActionSelect | VSCOSXEnvelopeViewMouseActionPersistentSelect))
    {
		//NSRect selectionRect = currentSelectionRect;
		CGFloat nx, ny, nw, nh;
		
		if (locationInView.x < self.currentSelectionOrigin.x)
        {
			nx = locationInView.x;
			nw = self.currentSelectionOrigin.x - locationInView.x;
		}
		else
        {
			nx = self.currentSelectionOrigin.x;
			nw = locationInView.x - self.currentSelectionOrigin.x;
		}
		
		if (locationInView.y < self.currentSelectionOrigin.y)
        {
			ny = locationInView.y;
			nh = self.currentSelectionOrigin.y - locationInView.y;
		}
		else
        {
			ny = self.currentSelectionOrigin.y;
			nh = locationInView.y - self.currentSelectionOrigin.y;
		}
		
		self.currentSelectionRect = NSMakeRect(nx, ny, nw, nh);
		
		_pointsInCurrentSelectionRect.clear();
		
		[self addPointsInRect:self.currentSelectionRect toEnvelopePointSet:_pointsInCurrentSelectionRect];
	}
	
	else if (_currentMouseAction & VSCOSXEnvelopeViewMouseActionMove)
    {
		
		_currentMouseAction = VSCOSXEnvelopeViewMouseActionMove;
		
		EnvelopePointSet::iterator it;
        
        Float valueDelta = _envelopeEditorGUIConfig->valueDeltaForPointDelta(deltaY);
        Float timeDelta = _envelopeEditorGUIConfig->timeDeltaForPointDelta(deltaX);
		
        EnvelopePoints moveList;
		for (it = _currentlySelectedPoints.begin(); it != _currentlySelectedPoints.end(); it++) {
            moveList.push_back(*it);
		}
        
        envelope->displacePoints(moveList, timeDelta, valueDelta);
	}
    
    [self purgeCurrentlySelectedPoints];
	
    [self.layer setNeedsDisplay];
}

@end
