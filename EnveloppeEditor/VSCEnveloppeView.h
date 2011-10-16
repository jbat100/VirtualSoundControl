//
//  EnveloppeView.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCEnveloppeViewSetup.h"
#import "VSCEnveloppe.h"
#import "VSCEnveloppePoint.h"

#include <list>
#include <set>
#include <boost/shared_ptr.hpp>

typedef enum _VSCEnveloppeViewClickArea {
	VSCEnveloppeViewClickAreaNone,
	VSCEnveloppeViewClickAreaSelection,
	VSCEnveloppeViewClickAreaPoint
} VSCEnveloppeViewClickArea;

/* 
 *  Using bitmask type as multiple actions are possible on click down (select, deselect, move, create)
 */
typedef enum _VSCEnveloppeViewMouseAction {
	VSCEnveloppeViewMouseActionNone = 0,
	VSCEnveloppeViewMouseActionSelect = 1 << 0,
    VSCEnveloppeViewMouseActionPersistentSelect = 1 << 1,
	VSCEnveloppeViewMouseActionCreate = 1 << 2,
    VSCEnveloppeViewMouseActionDelete = 1 << 3,
	VSCEnveloppeViewMouseActionMove = 1 << 4,
	VSCEnveloppeViewMouseActionCreateControl = 1 << 5,
	VSCEnveloppeViewMouseActionDeleteControl = 1 << 6,
    VSCEnveloppeViewMouseActionMoveControl = 1 << 7,
} VSCEnveloppeViewMouseAction;


@interface VSCEnveloppeView : NSView {
	
    /*
     *  The enveloppe that the view is representing
     */
	VSCEnveloppePtr _enveloppe;
	
    /*
     *  A view setup
     */
    VSCEnveloppeViewSetupPtr _enveloppeViewSetup;
	
	/*
     *  Keep track of the current grid points and their corresponding pixel
	 *  so that they do not need to be calculated on every draw, need updating
	 *	when frame/bounds change or view range/zoom changes
     */
	
    /*
     *  Keeps track of the currently selected points for group operations (move for example)
     */
	std::set<VSCEnveloppePointPtr> _currentlySelectedPoints;
	std::set<VSCEnveloppePointPtr> _pointsInCurrentSelectionRect;
	
    /*
     *  Keeps track of the current mouse action
     */
	VSCEnveloppeViewMouseAction currentMouseAction;
	BOOL movedSinceMouseDown;
    
    /*
     *  Keeps track of the selection rectangles
     *  - currentSelectionRect is the rect which should currently be affected by mouse movements (in case the currentMouseAction
     *  VSCEnveloppeViewMouseActionSelect)
     *  
     */
    NSRect currentSelectionRect;
	NSPoint currentSelectionOrigin;

}

/* 
 *  Basic C++ setters / getters *
 */
-(VSCEnveloppePtr)getEnveloppe;
-(void)setEnveloppe:(VSCEnveloppePtr)enveloppe; 
-(VSCEnveloppeViewSetupPtr) getEnveloppeViewSetup;
-(void)setEnveloppeViewSetup:(VSCEnveloppeViewSetupPtr)enveloppe; 
/* 
 *  Point C++ setters / getters *
 */
-(void)getCurrentlySelectedPoints:(std::set<VSCEnveloppePointPtr>&)points;
-(void)setCurrentlySelectedPoints:(std::set<VSCEnveloppePointPtr>&)points;

/* 
 * View/Enveloppe space conversion and manipulation tools 
 */
-(double) valueForPoint:(NSPoint)point;
-(double) timeForPoint:(NSPoint)point;
-(BOOL) point:(NSPoint)p touchesEnveloppePoint:(VSCEnveloppePointPtr)enveloppePoint;
-(NSPoint) pointForTime:(double)time value:(double)value;
-(NSPoint) pointForEnveloppePoint:(VSCEnveloppePointPtr)controlPoint;
-(void) setEnveloppePoint:(VSCEnveloppePointPtr)point withPoint:(NSPoint)p;
-(VSCEnveloppePointPtr) enveloppePointUnderPoint:(NSPoint)point;
-(void) getEnveloppePoints:(std::list<VSCEnveloppePointPtr>&)points InRect:(NSRect)rect; 
/* 
 * Automatic View Setup
 */
-(void)autoAdjustEnveloppeViewSetup;
/* 
 *  Create points 
 */
-(VSCEnveloppePointPtr) createEnveloppePointForPoint:(NSPoint)point;

@end


