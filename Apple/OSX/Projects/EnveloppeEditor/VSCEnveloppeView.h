//
//  EnveloppeView.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCEnveloppeLayer.h"

#import "VSCSound.h"
#import "VSCEnveloppe.h"
#import "VSCEnveloppePoint.h"
#import "VSCEnveloppeGUI.h"

#import <list>
#import <set>
#import <boost/shared_ptr.hpp>

@class VSCEnveloppeView;

@protocol VSCEnveloppeViewDataSource <NSObject>

-(VSC::Enveloppe::SPtr) mainEnveloppeForEnveloppeView:(VSCEnveloppeView*)enveloppeView;
-(VSC::Enveloppe::List) backgroundEnveloppesForEnveloppeView:(VSCEnveloppeView*)enveloppeView;

@end


typedef enum _VSCEnveloppeViewClickArea {
	VSCEnveloppeViewClickAreaNone,
	VSCEnveloppeViewClickAreaSelection,
	VSCEnveloppeViewClickAreaPoint
} VSCEnveloppeViewClickArea;

/* 
 *  Using bitmask type as multiple actions are possible on click down (select, deselect, move, create)
 */

typedef enum _VSC::EnveloppeViewMouseAction {
	VSCEnveloppeViewMouseActionNone             = 0,
	VSCEnveloppeViewMouseActionSelect           = 1 << 0,
    VSCEnveloppeViewMouseActionPersistentSelect = 1 << 1,
	VSCEnveloppeViewMouseActionCreate           = 1 << 2,
    VSCEnveloppeViewMouseActionDelete           = 1 << 3,
	VSCEnveloppeViewMouseActionMove             = 1 << 4,
	VSCEnveloppeViewMouseActionCreateControl    = 1 << 5,
	VSCEnveloppeViewMouseActionDeleteControl    = 1 << 6,
    VSCEnveloppeViewMouseActionMoveControl      = 1 << 7,
} VSCEnveloppeViewMouseAction;


@interface VSCEnveloppeView : NSView <VSCEnveloppeEditor>

@property (nonatomic, strong) VSCEnveloppeLayer* mainEnveloppeLayer;
@property (nonatomic, strong) VSCEnveloppeLayer* backgroundEnveloppesLayer;

@property (nonatomic, assign, readonly) NSRect currentSelectionRect;
@property (nonatomic, assign, readonly) NSPoint currentSelectionOrigin;

@property (nonatomic, assign) VSC::EnveloppeEditorGUIConfig::SPtr envelopeEditorGUIConfig;

@property (weak) id<VSCEnveloppeViewDataSource> dataSource;

-(void) reloadEnveloppes;
-(void) redrawMainEnveloppe;
-(void) redrawAllEnveloppes;

-(VSC::Enveloppe::PointSet&)getCurrentlySelectedPoints; // passing back by reference is less expensive
-(void)getCurrentlySelectedPoints:(VSC::Enveloppe::PointSet&)points; // copying into new set is more expensice
-(void)setCurrentlySelectedPoints:(VSC::Enveloppe::PointSet&)points;

/* 
 * View/Enveloppe space conversion and manipulation tools 
 */
-(BOOL) point:(NSPoint)p touchesEnveloppePoint:(VSC::EnveloppePoint::SPtr)enveloppePoint;
-(NSPoint) pointForTime:(Float)time value:(Float)value;
-(NSPoint) pointForEnveloppePoint:(VSC::EnveloppePoint::SPtr)controlPoint;
-(void) setEnveloppePoint:(VSC::EnveloppePoint::SPtr)point withPoint:(NSPoint)p;
-(VSC::EnveloppePoint::SPtr) enveloppePointUnderPoint:(NSPoint)point;
-(void) getEnveloppePoints:(VSC::Enveloppe::PointList&)points InRect:(NSRect)rect;

/* 
 * Automatic View Setup
 */
-(void)autoAdjustEnveloppeViewSetup;

/* 
 *  Create points 
 */
-(VSC::EnveloppePoint::SPtr) createEnveloppePointForPoint:(const NSPoint)point;

@end


