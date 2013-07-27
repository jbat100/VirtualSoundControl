//
//  VSC::EnvelopeLayer.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 4/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "VSCEnvelopeMinimal.h"

#import <map>


@protocol VSCOSXEnvelopeEditor <NSObject>

-(BOOL) envelopeIsEditable:(VSC::Envelope_SPtr)envelope;
-(BOOL) pointIsSelected:(VSC::EnvelopePoint_SPtr)envelopePoint;
-(NSRect) currentSelectionRectForEnvelope:(VSC::Envelope_SPtr)envelope;

-(VSC::EnvelopeEditorGUIConfig_SPtr) envelopeEditorGUIConfig;
-(VSC::EnvelopeGUIConfig_SPtr) sender:(id) requestsGUIConfigForEnvelope:(VSC::Envelope_SPtr)envelope;

@end

@interface VSCOSXEnvelopeLayer : CALayer 

@property (weak) id<VSCOSXEnvelopeEditor> editor;

-(void) addEnvelope:(VSC::Envelope_SPtr)envelope;
-(void) addEnvelope:(VSC::Envelope_SPtr)envelope atIndex:(NSUInteger)index;
-(void) removeEnvelope:(VSC::Envelope_SPtr)envelope;

//-(NSUInteger) numberOfEnvelop

@end
