//
//  VSC::EnvelopeLayer.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 4/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "VSCEnvelope.h"
#import "VSCEnvelopeGUI.h"

#import <map>


@protocol VSCOSXEnvelopeEditor <NSObject>

-(BOOL) envelopeIsEditable:(VSC::Envelope::SPtr)envelope;
-(BOOL) pointIsSelected:(VSC::EnvelopePoint::SPtr)envelopePoint;
-(NSRect) currentSelectionRectForEnvelope:(VSC::Envelope::SPtr)envelope;

-(VSC::EnvelopeEditorGUIConfig::SPtr) envelopeEditorGUIConfig;
-(VSC::EnvelopeGUIConfig::SPtr) sender:(id) requestsGUIConfigForEnvelope:(VSC::Envelope::SPtr)envelope;

@end

@interface VSCOSXEnvelopeLayer : CALayer 

@property (weak) id<VSCOSXEnvelopeEditor> editor;

-(void) addEnvelope:(VSC::Envelope::SPtr)envelope;
-(void) addEnvelope:(VSC::Envelope::SPtr)envelope atIndex:(NSUInteger)index;
-(void) removeEnvelope:(VSC::Envelope::SPtr)envelope;

-(NSUInteger) numberOfEnvelop

@end
