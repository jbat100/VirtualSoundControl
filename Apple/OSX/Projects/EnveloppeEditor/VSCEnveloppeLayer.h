//
//  VSCEnveloppeLayer.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "VSCEnveloppe.h"
#import "VSCEnveloppeGUI.h"

#import <map>

typedef std::map<VSCEnveloppePtr, VSCEnveloppeGUIConfigPtr>      VSCEnveloppeGUIConfigMap;

@protocol VSCEnveloppeEditor <NSObject>

-(BOOL) enveloppeIsEditable:(VSCEnveloppePtr)enveloppe;
-(BOOL) pointIsSelected:(VSCEnveloppePointPtr)enveloppePoint;
-(NSRect) currentSelectionRectForEnveloppe:(VSCEnveloppePtr)enveloppe;

@end

@interface VSCEnveloppeLayer : CALayer {
    
    @private
    
    VSCEnveloppe::List          _enveloppeList;
    
    VSCEnveloppe::ValueRange    _valueRange;
    VSCEnveloppe::TimeRange     _timeRange;
    
    VSCEnveloppeGUIConfigPtr     _defaultDisplaySetup;
    VSCEnveloppeGUIConfigMap     _enveloppeDisplaySetupMap;
    
}

@property (nonatomic, weak) id<VSCEnveloppeEditor> editor;

-(void) addEnveloppe:(VSCEnveloppePtr)enveloppe;
-(void) addEnveloppe:(VSCEnveloppePtr)enveloppe atIndex:(NSUInteger)index;
-(void) removeEnveloppe:(VSCEnveloppePtr)enveloppe;

-(VSCEnveloppe::ValueRange) getValueRange;
-(void) setValueRange:(VSCEnveloppe::ValueRange)range;

-(VSCEnveloppe::TimeRange) getTimeRange;
-(void) setTimeRange:(VSCEnveloppe::TimeRange)range;

-(VSCEnveloppeGUIConfigPtr) getDefaultDisplaySetup;
-(void) setDefaultDisplaySetup:(VSCEnveloppeGUIConfigPtr)setup;

-(void) setDisplaySetup:(VSCEnveloppeGUIConfigPtr)setup forEnveloppe:(VSCEnveloppePtr)enveloppe;
-(VSCEnveloppeGUIConfigPtr) getDisplaySetupForEnveloppe:(VSCEnveloppePtr)enveloppe;


@end
