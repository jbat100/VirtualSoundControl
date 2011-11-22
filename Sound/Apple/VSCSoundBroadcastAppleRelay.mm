//
//  VSCSoundBroadcastAppleRelay.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/22/11.
//  Copyright (c) 2011 NXP. All rights reserved.
//

#import "VSCSoundBroadcastAppleRelay.h"

VSCSoundBroadcastAppleRelay::VSCSoundBroadcastAppleRelay() {
    parameterListeners = [[NSMutableSet alloc] initWithCapacity:10];
    propertyListeners = [[NSMutableSet alloc] initWithCapacity:10];
}

VSCSoundBroadcastAppleRelay::~VSCSoundBroadcastAppleRelay() {
    [parameterListeners release];
    [propertyListeners release];
}

void VSCSoundBroadcastAppleRelay::parameterChanged(VSCSoundParameterizedElement* element, VSCSParameter::Key k, double value) {
    for (id<VSCSParameterListener> listener in parameterListeners) {
        [listener parameterWithKey:k changedTo:value forElement:element];
    }
}

void VSCSoundBroadcastAppleRelay::propertyChanged(VSCSoundPropertizedElement* element, VSCSProperty::Key k, std::string value) {
    for (id<VSCSPropertyListener> listener in propertyListeners) {
        [listener propertyWithKey:k changedTo:value forElement:element];
    }
}

void VSCSoundBroadcastAppleRelay::addParameterListener(id<VSCSParameterListener> parameterListener) {
    [parameterListeners addObject:parameterListener];
}

void VSCSoundBroadcastAppleRelay::removeParameterListener(id<VSCSParameterListener> parameterListener) {
    [parameterListeners removeObject:parameterListener];
}

void VSCSoundBroadcastAppleRelay::addPropertyListener(id<VSCSPropertyListener> propertyListener) {
    [propertyListeners addObject:propertyListener];
}

void VSCSoundBroadcastAppleRelay::removePropertyListener(id<VSCSPropertyListener> propertyListener) {
    [propertyListeners removeObject:propertyListener];
}
