//
//  EnveloppeView.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

class VSCEnveloppeViewSetup;
class VSCEnveloppe;
class VSCEnveloppePoint;

#import <list>


@interface VSCEnveloppeView : NSView {
	
	VSCEnveloppe* _enveloppe;
	VSCEnveloppeViewSetup* _enveloppeViewSetup;
	std::list<VSCEnveloppePoint*> _currentlySelectedPoints;
	BOOL _performingSelection;
	NSRect _selectionRect;

}

/* C++ setters / getters */
-(VSCEnveloppe*) enveloppe;
-(void) setEnveloppe(VSCEnveloppe* enveloppe); // class takes responsability for deleting on dealloc
-(VSCEnveloppeViewSetup*) enveloppeViewSetup;
-(void) setEnveloppeViewSetup(VSCEnveloppeViewSetup* enveloppe); // class takes responsability for deleting on dealloc
-(std::list<VSCEnveloppePoint*>*) currentlySelectedPoints;


-(double) valueForPoint:(NSPoint)point;
-(double) timeForPoint:(NSPoint)point;
-(NSPoint) pointForTime:(double)time value:(double)value;
-(NSPoint) pointForControlPoint:(VSCEnveloppePoint*)controlPoint;
-(void) setControlPoint:(VSCEnveloppePoint*)point withPoint:(NSPoint)p;
-(VSCEnveloppePoint*) controlPointForPoint:(NSPoint)point;

@end
