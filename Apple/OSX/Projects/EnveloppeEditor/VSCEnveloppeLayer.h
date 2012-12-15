//
//  VSC::EnveloppeLayer.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "VSCEnveloppe.h"
#import "VSCEnveloppeGUI.h"

#import <map>


@protocol VSCEnveloppeEditor <NSObject>

-(BOOL) enveloppeIsEditable:(VSC::Enveloppe::SPtr)enveloppe;
-(BOOL) pointIsSelected:(VSC::EnveloppePoint::SPtr)enveloppePoint;
-(NSRect) currentSelectionRectForEnveloppe:(VSC::Enveloppe::SPtr)enveloppe;

-(VSC::EnveloppeEditorGUIConfig::SPtr) enveloppeEditorGUIConfig;
-(VSC::EnveloppeGUIConfig::SPtr) sender:(id) requestsGUIConfigForEnveloppe:(VSC::Enveloppe::SPtr)enveloppe;

@end

@interface VSCEnveloppeLayer : CALayer 

@property (weak) id<VSCEnveloppeEditor> editor;

-(void) addEnveloppe:(VSC::Enveloppe::SPtr)enveloppe;
-(void) addEnveloppe:(VSC::Enveloppe::SPtr)enveloppe atIndex:(NSUInteger)index;
-(void) removeEnveloppe:(VSC::Enveloppe::SPtr)enveloppe;

@end
