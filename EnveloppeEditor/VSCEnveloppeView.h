//
//  EnveloppeView.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCEnveloppeViewSetup.h";
#import "VSCEnveloppe.h";
#import "VSCEnveloppePoint.h";

#include <list>
#include <set>

typedef enum _VSCEnveloppeViewClickArea {
	VSCEnveloppeViewClickAreaNone,
	VSCEnveloppeViewClickAreaSelection,
	VSCEnveloppeViewClickAreaPoint
} VSCEnveloppeViewClickArea;


typedef enum _VSCEnveloppeViewMouseAction {
	VSCEnveloppeViewMouseActionNone,
	VSCEnveloppeViewMouseActionSelectPoints,
	VSCEnveloppeViewMouseActionCreatePoints,
	VSCEnveloppeViewMouseActionMovePoints,
	VSCEnveloppeViewMouseActionCreateControlPoints,
	VSCEnveloppeViewMouseActionMoveControlPoints
} VSCEnveloppeViewMouseAction;

@interface VSCEnveloppeView : NSView {
	
	VSCEnveloppe* _enveloppe;
	VSCEnveloppeViewSetup* _enveloppeViewSetup;
	
	std::set<VSCEnveloppePointPtr> _currentlySelectedPoints;
	
	VSCEnveloppeViewMouseAction currentMouseAction;
	BOOL movedSinceMouseDown;
	NSMutableArray* selectionRects; // there can be more than one selection rect!

}

@property (nonatomic, retain) NSMutableArray* selectionRects;

/* C++ setters / getters */
-(VSCEnveloppe*)getEnveloppe;
-(void)setEnveloppe:(VSCEnveloppe*)enveloppe; // class takes responsability for deleting on dealloc
-(VSCEnveloppeViewSetup*) getEnveloppeViewSetup;
-(void)setEnveloppeViewSetup:(VSCEnveloppeViewSetup*)enveloppe; // class takes responsability for deleting on dealloc
-(void)getCurrentlySelectedPoints:(std::set<VSCEnveloppePointPtr>&)points;
-(void)setCurrentlySelectedPoints:(std::set<VSCEnveloppePointPtr>&)points;


-(double) valueForPoint:(NSPoint)point;
-(double) timeForPoint:(NSPoint)point;
-(BOOL) point:(NSPoint)p touchesEnveloppePoint:(VSCEnveloppePointPtr)enveloppePoint;
-(NSPoint) pointForTime:(double)time value:(double)value;
-(NSPoint) pointForEnveloppePoint:(VSCEnveloppePointPtr)controlPoint;
-(void) setEnveloppePoint:(VSCEnveloppePointPtr)point withPoint:(NSPoint)p;
-(VSCEnveloppePointPtr) enveloppePointUnderPoint:(NSPoint)point;
-(void) getEnveloppePoints:(std::list<VSCEnveloppePointPtr>&)points InRect:(NSRect)rect; 


/* create points */
-(VSCEnveloppePointPtr) createEnveloppePointForPoint:(NSPoint)point;

@end
