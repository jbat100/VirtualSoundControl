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


@interface VSCEnveloppeView : NSView {
	
	VSCEnveloppe* _enveloppe;
	VSCEnveloppeViewSetup* _enveloppeViewSetup;
	
	std::set<VSCEnveloppePoint*> _currentlySelectedPoints;
	
	VSCEnveloppeViewClickArea clickArea;
	BOOL movedSinceMouseDown;
	NSRect selectionRect;

}

@property (nonatomic, assign) BOOL performingSelection;
@property (nonatomic, assign) NSRect selectionRect;

/* C++ setters / getters */
-(VSCEnveloppe*)getEnveloppe;
-(void)setEnveloppe:(VSCEnveloppe*)enveloppe; // class takes responsability for deleting on dealloc
-(VSCEnveloppeViewSetup*) getEnveloppeViewSetup;
-(void)setEnveloppeViewSetup:(VSCEnveloppeViewSetup*)enveloppe; // class takes responsability for deleting on dealloc
-(std::set<VSCEnveloppePoint*>&) getCurrentlySelectedPoints;
-(void)setCurrentlySelectedPoints:(std::set<VSCEnveloppePoint*>)points;


-(double) valueForPoint:(NSPoint)point;
-(double) timeForPoint:(NSPoint)point;
-(BOOL) point:(NSPoint)p touchesEnveloppePoint:(VSCEnveloppePoint*)enveloppePoint;
-(NSPoint) pointForTime:(double)time value:(double)value;
-(NSPoint) pointForEnveloppePoint:(VSCEnveloppePoint*)controlPoint;
-(void) setEnveloppePoint:(VSCEnveloppePoint*)point withPoint:(NSPoint)p;
-(VSCEnveloppePoint*) controlPointForPoint:(NSPoint)point;

-(std::list<VSCEnveloppePoint*>) enveloppePointsInRect:(NSRect)rect; 

@end
