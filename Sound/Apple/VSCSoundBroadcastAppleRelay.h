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
#import "VSCSoundParameterized.h"
#import "VSCSoundPropertized.h"

#import <string>

@protocol VSCSParameterAppleListener <NSObject>

-(BOOL) interestedInParameterId:(VSCSParameterId)paramId;

-(void) parameterWithKey:(VSCSParameter::Key)key 
			   changedTo:(double)value 
			  forElement:(VSCSoundParameterized*)element;

@end

@protocol VSCSPropertyAppleListener <NSObject>

-(BOOL) interestedInPropertyId:(VSCSPropertyId)propId;

-(void) propertyWithKey:(VSCSProperty::Key)key 
			  changedTo:(std::string)value 
			 forElement:(VSCSoundPropertized*)element;

@end

class VSCSoundBroadcastAppleRelay : public VSCSParameterListener, public VSCSPropertyListener {
    
    VSCSoundBroadcastAppleRelay();
    ~VSCSoundBroadcastAppleRelay();
    
    void addParameterAppleListener(id<VSCSParameterAppleListener> parameterListener);
    void removeParameterAppleListener(id<VSCSParameterAppleListener> parameterListener);
    
    void addPropertyAppleListener(id<VSCSPropertyAppleListener> propertyListener);
    void removePropertyAppleListener(id<VSCSPropertyAppleListener> propertyListener);
	
	virtual void parameterChanged(VSCSoundParameterized* element, VSCSParameter::Key, double value);
	virtual void propertyChanged(VSCSoundPropertized* element, VSCSProperty::Key, std::string value);
    
protected:
    
    NSMutableSet* parameterListeners;
    NSMutableSet* propertyListeners;

};