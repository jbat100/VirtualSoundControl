//
//  EnvelopeView.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCEnvelopeLayer.h"

#import "VSCSound.h"
#import "VSCEnvelope.h"
#import "VSCEnvelopePoint.h"
#import "VSCEnvelopeGUI.h"

#import <list>
#import <set>
#import <boost/shared_ptr.hpp>

@class VSCEnvelopeView;

@protocol VSCEnvelopeViewDataSource <NSObject>

-(VSC::Envelope::SPtr) mainEnvelopeForEnvelopeView:(VSCEnvelopeView*)envelopeView;
-(VSC::Envelope::List) backgroundEnvelopesForEnvelopeView:(VSCEnvelopeView*)envelopeView;

@end


typedef enum _VSCEnvelopeViewClickArea {
	VSCEnvelopeViewClickAreaNone,
	VSCEnvelopeViewClickAreaSelection,
	VSCEnvelopeViewClickAreaPoint
} VSCEnvelopeViewClickArea;

/* 
 *  Using bitmask type as multiple actions are possible on click down (select, deselect, move, create)
 */

typedef enum _VSC::EnvelopeViewMouseAction {
	VSCEnvelopeViewMouseActionNone             = 0,
	VSCEnvelopeViewMouseActionSelect           = 1 << 0,
    VSCEnvelopeViewMouseActionPersistentSelect = 1 << 1,
	VSCEnvelopeViewMouseActionCreate           = 1 << 2,
    VSCEnvelopeViewMouseActionDelete           = 1 << 3,
	VSCEnvelopeViewMouseActionMove             = 1 << 4,
	VSCEnvelopeViewMouseActionCreateControl    = 1 << 5,
	VSCEnvelopeViewMouseActionDeleteControl    = 1 << 6,
    VSCEnvelopeViewMouseActionMoveControl      = 1 << 7,
} VSCEnvelopeViewMouseAction;


@interface VSCEnvelopeView : NSView <VSCEnvelopeEditor>

@property (nonatomic, strong) VSCEnvelopeLayer* mainEnvelopeLayer;
@property (nonatomic, strong) VSCEnvelopeLayer* backgroundEnvelopesLayer;

@property (nonatomic, assign, readonly) NSRect currentSelectionRect;
@property (nonatomic, assign, readonly) NSPoint currentSelectionOrigin;

@property (nonatomic, assign) VSC::EnvelopeEditorGUIConfig::SPtr envelopeEditorGUIConfig;

@property (weak) id<VSCEnvelopeViewDataSource> dataSource;

-(void) reloadEnvelopes;
-(void) redrawMainEnvelope;
-(void) redrawAllEnvelopes;

-(VSC::Envelope::PointSet&)getCurrentlySelectedPoints; // passing back by reference is less expensive
-(void)getCurrentlySelectedPoints:(VSC::Envelope::PointSet&)points; // copying into new set is more expensice
-(void)setCurrentlySelectedPoints:(VSC::Envelope::PointSet&)points;

/* 
 * View/Envelope space conversion and manipulation tools 
 */
-(BOOL) point:(NSPoint)p touchesEnvelopePoint:(VSC::EnvelopePoint::SPtr)envelopePoint;
-(NSPoint) pointForTime:(Float)time value:(Float)value;
-(NSPoint) pointForEnvelopePoint:(VSC::EnvelopePoint::SPtr)controlPoint;
-(void) setEnvelopePoint:(VSC::EnvelopePoint::SPtr)point withPoint:(NSPoint)p;
-(VSC::EnvelopePoint::SPtr) envelopePointUnderPoint:(NSPoint)point;
-(void) getEnvelopePoints:(VSC::Envelope::PointList&)points InRect:(NSRect)rect;

/* 
 * Automatic View Setup
 */
-(void)autoAdjustEnvelopeViewSetup;

/* 
 *  Create points 
 */
-(VSC::EnvelopePoint::SPtr) createEnvelopePointForPoint:(const NSPoint)point;

@end


