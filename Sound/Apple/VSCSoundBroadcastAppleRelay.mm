//
//  VSCSoundBroadcastAppleRelay.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/22/11.
//  Copyright (c) 2011 NXP. All rights reserved.
//

#import "VSCSoundBroadcastAppleRelay.h"

#import "NSString+VSCAdditions.h"

VSCSoundBroadcastAppleRelay::VSCSoundBroadcastAppleRelay() {
    broadcastListeners = [[NSMutableSet alloc] initWithCapacity:10];
}

VSCSoundBroadcastAppleRelay::~VSCSoundBroadcastAppleRelay() {
    [broadcastListeners release];
}

void VSCSoundBroadcastAppleRelay::parameterChanged(VSCSParameterId paramId, double value) {
    for (id<VSCSBroadcastAppleListener> listener in broadcastListeners) {
        [listener parameterId:paramId changedTo:value];
    }
}

void VSCSoundBroadcastAppleRelay::indexedParameterChanged(VSCSIndexedParameterId paramId, double value) {
    for (id<VSCSBroadcastAppleListener> listener in broadcastListeners) {
        [listener indexedParameterId:paramId changedTo:value];
    }
}


void VSCSoundBroadcastAppleRelay::propertyChanged(VSCSPropertyId propId, std::string value) {
    for (id<VSCSBroadcastAppleListener> listener in broadcastListeners) {
		NSString* nsString = [NSString stringWithStdString:value];
        [listener propertyId:propId changedTo:nsString];
    }
}

void VSCSoundBroadcastAppleRelay::addBroadcastAppleListener(id<VSCSBroadcastAppleListener> broadcastListener) {
    [broadcastListeners addObject:broadcastListener];
}

void VSCSoundBroadcastAppleRelay::removeBroadcastAppleListener(id<VSCSBroadcastAppleListener> broadcastListener) {
    [broadcastListeners removeObject:broadcastListener];
}

