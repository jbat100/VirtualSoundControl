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

#import <cmath>
#import <boost/assert.hpp>

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

@interface VSCEnvelopeView ()
{
    /*
     *  Keeps track of the currently selected points for group operations (move for example)
     *  A set is more appropriate than a list as we don't care about ordering and adding
     *  without needing to check for presence (sets cannot have duplicates) is an advantage
     */
    Envelope::PointSet _currentlySelectedPoints;
	Envelope::PointSet _pointsInCurrentSelectionRect;
    
}

@property (nonatomic, assign) VSCEnvelopeViewMouseAction currentMouseAction;
@property (nonatomic, assign) BOOL movedSinceMouseDown;

@property (nonatomic, assign, readwrite) NSRect currentSelectionRect;
@property (nonatomic, assign, readwrite) NSPoint currentSelectionOrigin;

-(void) purgeCurrentlySelectedPoints;
-(void) addPointsInRect:(NSRect)rect toPointSet:(std::set<EnvelopePoint::SPtr>&)pointSet;

@end



@implementation VSCEnvelopeView


#pragma mark - NSView Methods

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        _envelope = Envelope::SPtr();
		_envelopeEditorGUIConfig = EnvelopeEditorGUIConfig::SPtr(new EnvelopeEditorGUIConfig());
    }
    return self;
}

-(void)setFrameSize:(NSSize)newSize
{
    _envelopeEditorGUIConfig->setEditorSize(MakeSizeFromSize(newSize));
    [super setFrameSize:newSize];
}

#pragma mark - Custom Draw Methods

-(void) reloadEnvelopes
{    
    _envelope = Envelope::SPtr();
    _backgroundEnvelopeList.clear();
    
    if (self.dataSource)
    {
        _envelope = [self.dataSource mainEnvelopeForEnvelopeView:self];
        _backgroundEnvelopeList = [self.dataSource backgroundEnvelopesForEnvelopeView:self];
        [self.mainEnvelopeLayer addEnvelope:_envelope];
    }
    
}

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
    
    if (!_envelope)
    {
        _currentlySelectedPoints.clear();
        _pointsInCurrentSelectionRect.clear();
    }
    
    for (Envelope::PointSet::iterator setIt = _currentlySelectedPoints.begin(); setIt != _currentlySelectedPoints.end(); setIt++)
    {
        Envelope::ConstPointIterator beginEnvPntIt = _envelope->getPointBeginConstIterator();
        Envelope::ConstPointIterator endEnvPntIt = _envelope->getPointEndConstIterator();
        Envelope::ConstPointIterator envPntIt = std::find(beginEnvPntIt, endEnvPntIt, *setIt);
        if (envPntIt == endEnvPntIt)
        {
            _currentlySelectedPoints.erase(setIt);
        }
    }
    
    for (Envelope::PointSet::iterator setIt = _pointsInCurrentSelectionRect.begin(); setIt != _pointsInCurrentSelectionRect.end(); setIt++)
    {
        Envelope::ConstPointIterator beginEnvPntIt = _envelope->getPointBeginConstIterator();
        Envelope::ConstPointIterator endEnvPntIt = _envelope->getPointEndConstIterator();
        Envelope::ConstPointIterator envPntIt = std::find(beginEnvPntIt, endEnvPntIt, *setIt);
        if (envPntIt == endEnvPntIt)
        {
            _pointsInCurrentSelectionRect.erase(setIt);
        }
    }
}

-(BOOL) envelopeIsEditable:(Envelope::SPtr)envelope
{
    if (envelope == _envelope)
    {
        return YES;
    }
    return NO;
}

-(BOOL) pointIsSelected:(EnvelopePoint::SPtr)envelopePoint
{
    Envelope::PointSet::const_iterator pointIt = _pointsInCurrentSelectionRect.find(envelopePoint);
    if (pointIt != _pointsInCurrentSelectionRect.end())
    {
        return YES;
    }
    return NO;
}

-(NSRect) currentSelectionRectForEnvelope:(Envelope::SPtr)envelope
{
    if (envelope == _envelope)
    {
        return self.currentSelectionRect;
    }
    return NSZeroRect;
}

-(EnvelopeEditorGUIConfig::SPtr) envelopeEditorGUIConfig
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
	
	if (!_envelope)
    {
		_envelopeEditorGUIConfig->setToDefault();
	}
    
    Float minTime, maxTime, minValue, maxValue;
    
    try
    {
        minTime = _envelope->minTime();
        maxTime = _envelope->maxTime();
        minValue = _envelope->minValue();
        maxValue = _envelope->maxValue();
        
        Float timeMargin = std::abs((maxTime-minTime)*0.2); 
        Float valueMargin = std::abs((maxValue-minValue)*0.2); 
        
        //minTime -= timeMargin
        maxTime += timeMargin;
        minValue -= valueMargin;
        maxValue += valueMargin;
    } 
    catch (EnvelopeEmptyException& e)
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

-(Envelope::SPtr) getEnvelope
{
    return _envelope;
}

-(void) setEnvelope:(Envelope::SPtr)envelope
{
	_currentlySelectedPoints.clear();
	_pointsInCurrentSelectionRect.clear();
    _envelope = envelope;
	//[self setNeedsDisplay:YES];
    [self.layer setNeedsDisplay];
}

-(EnvelopeEditorGUIConfig::SPtr) getEnvelopeViewSetup
{
    return _envelopeEditorGUIConfig;
}

-(void) setEnvelopeViewSetup:(EnvelopeEditorGUIConfig::SPtr)envelopeViewSetup
{
	BOOST_ASSERT(envelopeViewSetup);
    _envelopeEditorGUIConfig = envelopeViewSetup;
}

-(Envelope::PointSet&)getCurrentlySelectedPoints
{
    return _currentlySelectedPoints;
}

-(void)getCurrentlySelectedPoints:(Envelope::PointSet&)points
{
    for (Envelope::PointSet::iterator it = _currentlySelectedPoints.begin(); it != _currentlySelectedPoints.end(); it++)
    {
		points.insert(*it);
	}
}

-(void)setCurrentlySelectedPoints:(Envelope::PointSet&)points
{
    _currentlySelectedPoints.clear();
    for (Envelope::PointSet::iterator it = points.begin(); it != points.end(); it++)
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

-(EnvelopePoint::SPtr) envelopePointUnderPoint:(NSPoint)point
{
	if (!_envelope) return EnvelopePoint::SPtr();
	
	for (Envelope::ConstPointIterator it = _envelope->getPointBeginIterator(); it !=_envelope->getPointEndIterator(); it++)
    {
		if ([self point:point touchesEnvelopePoint:(*it)]) return (*it);
	}
	return EnvelopePoint::SPtr();
}

-(void) getEnvelopePoints:(Envelope::PointList&)ps InRect:(NSRect)rect
{
	if (!_envelope) return;
    
	for (Envelope::ConstPointIterator it = _envelope->getPointBeginIterator(); it !=_envelope->getPointEndIterator(); it++)
    {
		NSPoint p = [self pointForEnvelopePoint:(*it)];
		
        if (CGRectContainsPoint(rect, p)) ps.push_back(*it);
	}
}

-(void) addPointsInRect:(NSRect)rect toPointSet:(Envelope::PointSet&)pointSet
{
    Envelope::PointList rectPoints; 
	[self getEnvelopePoints:rectPoints InRect:rect];
    
	for (Envelope::PointIterator it = rectPoints.begin(); it != rectPoints.end(); it++)
    {
		pointSet.insert(*it);
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
    
    movedSinceMouseDown = NO;
	
	NSLog(@"Mouse down at x: %f, y: %f", locationInView.x, locationInView.y);
	
	if (!_envelope)
    {
		currentMouseAction = EnvelopeViewMouseActionNone;
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
		currentMouseAction = EnvelopeViewMouseActionPersistentSelect;
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
				currentMouseAction = EnvelopeViewMouseActionNone;
			}
			else
            {
				currentMouseAction = EnvelopeViewMouseAction(EnvelopeViewMouseActionCreate | EnvelopeViewMouseActionSelect);
				_currentlySelectedPoints.clear();
			}
		}
		
		/*
		 *	If mouse click was done over an envelope point, the select the point and wait
		 *	wait for further action (mouse up or mouse dragged) to determine whether to delete or move
		 */
		else
        {
			currentMouseAction = EnvelopeViewMouseAction(EnvelopeViewMouseActionDelete | EnvelopeViewMouseActionMove);
			_currentlySelectedPoints.insert(envelopePoint);
		}
		
	}
		
	if (currentMouseAction & (EnvelopeViewMouseActionSelect | EnvelopeViewMouseActionPersistentSelect))
    {
		self.currentSelectionRect = NSMakeRect(locationInView.x, locationInView.y, 0.0, 0.0);
		self.currentSelectionOrigin = NSMakePoint(locationInView.x, locationInView.y);
	}
	
    [self setNeedsDisplay:YES];
}

-(void) mouseUp:(NSEvent *)event
{	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:[[self window] contentView]];
    [self setNeedsDisplay:YES];
	NSLog(@"Mouse up at x: %f, y: %f", locationInView.x, locationInView.y);
	
	if (!_envelope)
    {
		currentMouseAction = EnvelopeViewMouseActionNone;
		return;
    }
	
    EnvelopePoint::SPtr envelopePoint = [self envelopePointUnderPoint:locationInView];
    
	/*
	 *	If the mouse action contains create and the mouse has not moved since click down, then create
	 *	a new point at the click location
	 */
    
	if ((currentMouseAction & EnvelopeViewMouseActionCreate) && movedSinceMouseDown == NO)
    {
		NSAssert(!envelopePoint, @"Expected envelopePoint to be NULL");
        EnvelopeCoordinate::SPtr coord = _envelopeEditorGUIConfig->createEnvelopeCoordinateForPoint(MakePointFromPoint(locationInView));
        EnvelopePoint::SPtr newPoint = EnvelopePoint::SPtr(new EnvelopePoint(*coord));
		_envelope->addPoint(newPoint);
	}
	
	/*
	 *	If the mouse action is select or persistant select and the mouse has not moved since click down
	 *	then select/deselect the point at mouse location, if non-persistent select then deselect all other 
	 *	points, reset current selection rect
	 */
    
	if ((currentMouseAction & (EnvelopeViewMouseActionSelect | EnvelopeViewMouseActionPersistentSelect)) &&  movedSinceMouseDown == NO)
    {
		if (currentMouseAction & EnvelopeViewMouseActionSelect)
        {
			_currentlySelectedPoints.clear();
		}
		
		// If a point exists at the point where the click occured then select/deselect the point
		if (envelopePoint)
        {
			Envelope::PointSet::iterator iter = _currentlySelectedPoints.find(envelopePoint);
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
    
	if ((currentMouseAction & EnvelopeViewMouseActionDelete) && movedSinceMouseDown == NO)
    {
		NSAssert(envelopePoint, @"Expected envelopePoint to be non-NULL");
		if (envelopePoint)
        {
			_envelope->removePoint(envelopePoint);
			_currentlySelectedPoints.erase(envelopePoint);
			_pointsInCurrentSelectionRect.erase(envelopePoint);
		}
	}
	
	/*
	 *	If the mouse action is select or persistant select and the mouse has moved since click down
	 *	then add the points in currentSelectionRect to _currentlySelectedPoints, if non-persistent select then 
	 *	deselect all other points, reset current selection rect.
	 */
    
    if ((currentMouseAction & (EnvelopeViewMouseActionSelect | EnvelopeViewMouseActionPersistentSelect)) && movedSinceMouseDown == YES)
    {
		[self addPointsInRect:self.currentSelectionRect toPointSet:_currentlySelectedPoints];
		
		_pointsInCurrentSelectionRect.clear();
		
		self.currentSelectionRect = NSMakeRect(0.0, 0.0, 0.0, 0.0);
		self.currentSelectionOrigin = NSMakePoint(0.0, 0.0);
	}
	
	currentMouseAction = EnvelopeViewMouseActionNone;
	movedSinceMouseDown = NO;
    
    [self purgeCurrentlySelectedPoints];
    
    [self.layer setNeedsDisplay];
}

-(void) mouseDragged:(NSEvent *)event
{	
	movedSinceMouseDown = YES;
	
	NSPoint eventLocation = [event locationInWindow];
    NSPoint locationInView = [self convertPoint:eventLocation fromView:[[self window] contentView]];
	CGFloat deltaX = [event deltaX];
	CGFloat deltaY = -[event deltaY];
	
	NSLog(@"Mouse dragged to %@", NSStringFromPoint(locationInView));
	std::cout << _currentlySelectedPoints.size() << " selected point(s)\n";
	
	if (!_envelope)
    {
		currentMouseAction = EnvelopeViewMouseActionNone;
		return;
	}
	
	/*
	 *	Eliminate mouse actions which are impossible once the mouse has moved (create and delete)
	 */
    
	currentMouseAction = (EnvelopeViewMouseAction) (currentMouseAction & ~(EnvelopeViewMouseActionCreate | EnvelopeViewMouseActionDelete));
	
	if (currentMouseAction & (EnvelopeViewMouseActionSelect | EnvelopeViewMouseActionPersistentSelect))
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
		
		[self addPointsInRect:self.currentSelectionRect toPointSet:_pointsInCurrentSelectionRect];
	}
	
	else if (currentMouseAction & EnvelopeViewMouseActionMove)
    {
		
		currentMouseAction = EnvelopeViewMouseActionMove;
		
		Envelope::PointSet::iterator it;
        
        Float valueDelta = _envelopeEditorGUIConfig->valueDeltaForPointDelta(deltaY);
        Float timeDelta = _envelopeEditorGUIConfig->timeDeltaForPointDelta(deltaX);
		
        Envelope::PointList moveList;
		for (it = _currentlySelectedPoints.begin(); it != _currentlySelectedPoints.end(); it++) {
            moveList.push_back(*it);
		}
        
        _envelope->displacePoints(moveList, timeDelta, valueDelta);
	}
    
    [self purgeCurrentlySelectedPoints];
	
    [self.layer setNeedsDisplay];
}

@end
