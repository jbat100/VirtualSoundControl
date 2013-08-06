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
#import "VSCEnvelopeMinimal.h"
#import "VSCEnvelope.h"



@interface VSCOSXEnvelopeView : NSView <VSCOSXEnvelopeEditor>

@property (nonatomic, strong) VSCOSXEnvelopeLayer* mainEnvelopeLayer;
@property (nonatomic, strong) VSCOSXEnvelopeLayer* backgroundEnvelopesLayer;

@property (nonatomic, assign, readonly) NSRect currentSelectionRect;
@property (nonatomic, assign, readonly) NSPoint currentSelectionOrigin;

@property (nonatomic, assign) VSC::EnvelopeEditorGUIConfig_SPtr envelopeEditorGUIConfig;

@property (weak) id<VSCOSXEnvelopeController> controller;

-(void) redrawMainEnvelope;
-(void) redrawAllEnvelopes;

-(VSC::EnvelopePointSet&) getCurrentlySelectedPoints; // passing back by reference is less expensive
-(void) getCurrentlySelectedPoints:(VSC::EnvelopePointSet&)points; // copying into new set is more expensice
-(void) setCurrentlySelectedPoints:(VSC::EnvelopePointSet&)points;

/* 
 * View/Envelope space conversion and manipulation tools 
 */

-(BOOL) point:(NSPoint)p touchesEnvelopePoint:(VSC::EnvelopePoint_SPtr)envelopePoint;
-(NSPoint) pointForTime:(VSC::Float)time value:(VSC::Float)value;
-(NSPoint) pointForEnvelopePoint:(VSC::EnvelopePoint_SPtr)controlPoint;
-(void) setEnvelopePoint:(VSC::EnvelopePoint_SPtr)point withPoint:(NSPoint)p;
-(VSC::EnvelopePoint_SPtr) envelopePointUnderPoint:(NSPoint)point;
-(void) getEnvelopePoints:(VSC::EnvelopePoints&)points InRect:(NSRect)rect;

/* 
 * Automatic View Setup
 */

-(void)autoAdjustEnvelopeViewSetup;

/* 
 *  Create points 
 */

-(VSC::EnvelopePoint_SPtr) createEnvelopePointForPoint:(const NSPoint)point;

@end


