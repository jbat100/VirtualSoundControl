//
//  EnvelopeView.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCOSXEnvelopeLayer.h"
#import "VSCOSXEnvelopeController.h"

#import "VSCSound.h"
#import "VSCEnvelope.h"
#import "VSCEnvelopePoint.h"
#import "VSCEnvelopeGUI.h"



@interface VSCOSXEnvelopeView : NSView <VSCOSXEnvelopeEditor>

@property (nonatomic, strong) VSCOSXEnvelopeLayer* mainEnvelopeLayer;
@property (nonatomic, strong) VSCOSXEnvelopeLayer* backgroundEnvelopesLayer;

@property (nonatomic, assign, readonly) NSRect currentSelectionRect;
@property (nonatomic, assign, readonly) NSPoint currentSelectionOrigin;

@property (nonatomic, assign) VSC::EnvelopeEditorGUIConfig_SPtr envelopeEditorGUIConfig;

@property (weak) id<VSCOSXEnvelopeController> controller;

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
-(NSPoint) pointForTime:(VSC::Float)time value:(VSC::Float)value;
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


