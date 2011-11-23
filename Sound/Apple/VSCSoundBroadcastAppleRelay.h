//
//  VSCSoundBroadcastAppleRelay.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/22/11.
//  Copyright (c) 2011 NXP. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "VSCSound.h"
#import "VSCSoundBroadcaster.h"
#import "VSCSoundParameters.h"
#import "VSCSoundProperties.h"
#import "VSCSoundParameterizedElement.h"
#import "VSCSoundPropertizedElement.h"

#import <string>

@protocol VSCSParameterListener <NSObject>

-(void) interestedInParameterId:(VSCSParameterId)paramId;

-(void) parameterWithKey:(VSCSParameter::Key)key 
			   changedTo:(double)value 
			  forElement:(VSCSoundParameterizedElement*)element;

@end

@protocol VSCSPropertyListener <NSObject>

-(void) interestedInPropertyId:(VSCSPropertyId)propId;

-(void) propertyWithKey:(VSCSProperty::Key)key 
			  changedTo:(std::string)value 
			 forElement:(VSCSoundPropertizedElement*)element;

@end

class VSCSoundBroadcastAppleRelay {
    
    VSCSoundBroadcastAppleRelay();
    ~VSCSoundBroadcastAppleRelay();
    
    void parameterChanged(VSCSoundParameterizedElement* element, VSCSParameter::Key k, double value);
    void propertyChanged(VSCSoundPropertizedElement* element, VSCSProperty::Key k, std::string value);
    
    void addParameterListener(id<VSCSParameterListener> parameterListener);
    void removeParameterListener(id<VSCSParameterListener> parameterListener);
    
    void addPropertyListener(id<VSCSPropertyListener> propertyListener);
    void removePropertyListener(id<VSCSPropertyListener> propertyListener);
    
protected:
    
    NSMutableSet* parameterListeners;
    NSMutableSet* propertyListeners;

};