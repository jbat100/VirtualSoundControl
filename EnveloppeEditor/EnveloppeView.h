//
//  EnveloppeView.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@class EnveloppeViewSetup;
@class Enveloppe;
@class EnveloppeControlPoint;


@interface EnveloppeView : NSView {
	
	Enveloppe* enveloppe;
	
	EnveloppeViewSetup* enveloppeViewSetup;
	NSMutableArray* currentlySelectedPoints;
	
	BOOL performingSelection;
	NSRect selectionRect;

}


@property (nonatomic, assign) Enveloppe* enveloppe;

@property (nonatomic, retain) EnveloppeViewSetup* enveloppeViewSetup;
@property (nonatomic, retain) NSMutableArray* currentlySelectedPoints;

-(double) valueForPoint:(NSPoint)point;
-(double) timeForPoint:(NSPoint)point;
-(NSPoint) pointForTime:(NSTimeInterval)time value:(double)value;
-(NSPoint) pointForControlPoint:(EnveloppeControlPoint*)controlPoint;
-(void) setControlPoint:(EnveloppeControlPoint*)controlPoint withPoint:(NSPoint)p;
-(EnveloppeControlPoint*) controlPointForPoint:(NSPoint)point;

@end
