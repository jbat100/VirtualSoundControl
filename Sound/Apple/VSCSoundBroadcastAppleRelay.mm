//
//  VSCSoundBroadcastAppleRelay.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/22/11.
//  Copyright (c) 2011 NXP. All rights reserved.
//

#import "VSCSoundBroadcastAppleRelay.h"

VSCSoundBroadcastAppleRelay::VSCSoundBroadcastAppleRelay() {
	this->addPropertyKey(VSCSProperty::KeyAll);
	this->addParameterKey(VSCSParameter::KeyAll);
    parameterListeners = [[NSMutableSet alloc] initWithCapacity:10];
    propertyListeners = [[NSMutableSet alloc] initWithCapacity:10];
}

VSCSoundBroadcastAppleRelay::~VSCSoundBroadcastAppleRelay() {
    [parameterListeners release];
    [propertyListeners release];
}

void VSCSoundBroadcastAppleRelay::parameterChanged(VSCSoundParameterized* element, VSCSParameter::Key k, double value) {
    for (id<VSCSParameterAppleListener> listener in parameterListeners) {
        [listener parameterWithKey:k changedTo:value forElement:element];
    }
}

void VSCSoundBroadcastAppleRelay::propertyChanged(VSCSoundPropertized* element, VSCSProperty::Key k, std::string value) {
    for (id<VSCSPropertyAppleListener> listener in propertyListeners) {
        [listener propertyWithKey:k changedTo:value forElement:element];
    }
}

void VSCSoundBroadcastAppleRelay::addParameterAppleListener(id<VSCSParameterAppleListener> parameterListener) {
    [parameterListeners addObject:parameterListener];
}

void VSCSoundBroadcastAppleRelay::removeParameterAppleListener(id<VSCSParameterAppleListener> parameterListener) {
    [parameterListeners removeObject:parameterListener];
}

void VSCSoundBroadcastAppleRelay::addPropertyAppleListener(id<VSCSPropertyAppleListener> propertyListener) {
    [propertyListeners addObject:propertyListener];
}

void VSCSoundBroadcastAppleRelay::removePropertyAppleListener(id<VSCSPropertyAppleListener> propertyListener) {
    [propertyListeners removeObject:propertyListener];
}
