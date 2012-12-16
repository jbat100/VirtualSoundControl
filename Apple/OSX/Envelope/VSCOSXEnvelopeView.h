//
//  EnvelopeView.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCOSXEnvelopeLayer.h"

#import "VSCSound.h"
#import "VSCEnvelope.h"
#import "VSCEnvelopePoint.h"
#import "VSCEnvelopeGUI.h"

#import <list>
#import <set>
#import <boost/shared_ptr.hpp>

@class VSCOSXEnvelopeView;

@protocol VSCOSXEnvelopeViewDataSource <NSObject>

-(VSC::Envelope::SPtr) mainEnvelopeForEnvelopeView:(VSCOSXEnvelopeView*)envelopeView;
-(VSC::Envelope::List) backgroundEnvelopesForEnvelopeView:(VSCOSXEnvelopeView*)envelopeView;

@end


typedef enum _VSCOSXEnvelopeViewClickArea {
	VSCOSXEnvelopeViewClickAreaNone,
	VSCOSXEnvelopeViewClickAreaSelection,
	VSCOSXEnvelopeViewClickAreaPoint
} VSCOSXEnvelopeViewClickArea;

/* 
 *  Using bitmask type as multiple actions are possible on click down (select, deselect, move, create)
 */

typedef enum _VSC::EnvelopeViewMouseAction {
	VSCOSXEnvelopeViewMouseActionNone             = 0,
	VSCOSXEnvelopeViewMouseActionSelect           = 1 << 0,
    VSCOSXEnvelopeViewMouseActionPersistentSelect = 1 << 1,
	VSCOSXEnvelopeViewMouseActionCreate           = 1 << 2,
    VSCOSXEnvelopeViewMouseActionDelete           = 1 << 3,
	VSCOSXEnvelopeViewMouseActionMove             = 1 << 4,
	VSCOSXEnvelopeViewMouseActionCreateControl    = 1 << 5,
	VSCOSXEnvelopeViewMouseActionDeleteControl    = 1 << 6,
    VSCOSXEnvelopeViewMouseActionMoveControl      = 1 << 7,
} VSCOSXEnvelopeViewMouseAction;


@interface VSCOSXEnvelopeView : NSView <VSCOSXEnvelopeEditor>

@property (nonatomic, strong) VSCOSXEnvelopeLayer* mainEnvelopeLayer;
@property (nonatomic, strong) VSCOSXEnvelopeLayer* backgroundEnvelopesLayer;

@property (nonatomic, assign, readonly) NSRect currentSelectionRect;
@property (nonatomic, assign, readonly) NSPoint currentSelectionOrigin;

@property (nonatomic, assign) VSC::EnvelopeEditorGUIConfig::SPtr envelopeEditorGUIConfig;

@property (weak) id<VSCOSXEnvelopeViewDataSource> dataSource;

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


