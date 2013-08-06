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
-(const VSC::EnvelopePointSet&) selectedEnvelopePointsForEnvelope:(VSC::Envelope_SPtr)envelopePoint;
-(NSRect) currentSelectionRectForEnvelope:(VSC::Envelope_SPtr)envelope;

-(VSC::EnvelopeEditorGUIConfig_SPtr) envelopeEditorGUIConfig;
-(VSC::EnvelopeGUIConfig_SPtr) GUIConfigForEnvelope:(VSC::Envelope_SPtr)envelope;

@end

@interface VSCOSXEnvelopeLayer : CALayer 

@property (weak) id<VSCOSXEnvelopeEditor> editor;

@property (nonatomic, assign) VSC::EnvelopeGUIConfig_SPtr defaultEnvelopeGUIConfig;
@property (nonatomic, assign) VSC::EnvelopeEditorGUIConfig_SPtr defaultEnvelopeEditorGUIConfig;

-(VSC::Envelopes&) envelopes;

@end
