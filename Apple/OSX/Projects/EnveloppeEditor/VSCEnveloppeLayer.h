//
//  VSCEnveloppeLayer.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "VSCEnveloppe.h"
#import "VSCEnveloppeDisplaySetup.h"

#import <map>

typedef std::map<VSCEnveloppePtr, VSCEnveloppeDisplaySetupPtr>      VSCEnveloppeDisplaySetupMap;


@interface VSCEnveloppeLayer : CALayer {
    
    @private
    
    VSCEnveloppe::List          _enveloppeList;
    
    VSCEnveloppe::ValueRange    _valueRange;
    VSCEnveloppe::TimeRange     _timeRange;
    
    VSCEnveloppeDisplaySetupPtr     _defaultDisplaySetup;
    VSCEnveloppeDisplaySetupMap     _enveloppeDisplaySetupMap;
    
}

-(void) addEnveloppe:(VSCEnveloppePtr)enveloppe;
-(void) addEnveloppe:(VSCEnveloppePtr)enveloppe atIndex:(NSUInteger)index;
-(void) removeEnveloppe:(VSCEnveloppePtr)enveloppe;

-(VSCEnveloppe::ValueRange) getValueRange;
-(void) setValueRange:(VSCEnveloppe::ValueRange)range;

-(VSCEnveloppe::TimeRange) getTimeRange;
-(void) setTimeRange:(VSCEnveloppe::TimeRange)range;

-(VSCEnveloppeDisplaySetupPtr) getDefaultDisplaySetup;
-(void) setDefaultDisplaySetup:(VSCEnveloppeDisplaySetupPtr)setup;

-(void) setDisplaySetup:(VSCEnveloppeDisplaySetupPtr)setup forEnveloppe:(VSCEnveloppePtr)enveloppe;
-(VSCEnveloppeDisplaySetupPtr) getDisplaySetupForEnveloppe:(VSCEnveloppePtr)enveloppe;

@end
