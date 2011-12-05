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

@protocol VSCSBroadcastAppleListener <NSObject>

-(void) parameterId:(VSCSParameterId)paramId changedTo:(double)value;
-(void) indexedParameterId:(VSCSIndexedParameterId)paramId changedTo:(double)value;
-(void) propertyId:(VSCSPropertyId)propId changedTo:(NSString*)value;

@end


class VSCSoundBroadcastAppleRelay : public VSCSBroadcastListener {
    
    VSCSoundBroadcastAppleRelay();
    ~VSCSoundBroadcastAppleRelay();
    
    void addBroadcastAppleListener(id<VSCSParameterAppleListener> parameterListener);
    void removeBroadcastAppleListener(id<VSCSParameterAppleListener> parameterListener);
	
	virtual void parameterChanged(VSCSParameterId paramId, double value);
	virtual void indexedParameterChanged(VSCSIndexedParameterId paramId, double value);
	virtual void propertyChanged(VSCSPropertyId propId, std::string value);
    
protected:
    
    NSMutableSet* broadcastListeners;

};