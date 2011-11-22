/*
 *  VSCSoundBroadcaster.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundBroadcaster.h"

#pragma mark - VSCSParameterListener

void VSCSParameterListener::addParameterizedElement(VSCSoundParameterizedElement* element) {
    _parameterizedElements.insert(element);
}

void VSCSParameterListener::removeParameterizedElement(VSCSoundParameterizedElement* element) {
    _parameterizedElements.erase(element);
}

std::set<VSCSoundParameterizedElement*> VSCSParameterListener::getParameterizedElements(void) {
    return _parameterizedElements;
}

void VSCSParameterListener::addParameterKey(VSCSParameter::Key k) {
    _parameterKeys.insert(k);
}

void VSCSParameterListener::removeParameterKey(VSCSParameter::Key k) {
    _parameterKeys.erase(k);
}

std::set<VSCSParameter::Key> VSCSParameterListener::getParameterKeys(void) {
    return  _parameterKeys;
}

void VSCSParameterListener::setMinimumRate(VSCSFloat r) {
    _minimumRate = r;
}

VSCSFloat VSCSParameterListener::getMinimumRate(void) {
    return _minimumRate;
}

#pragma mark - VSCSPropertyListener

void VSCSPropertyListener::addPropertizedElement(VSCSoundPropertizedElement* element) {
    _propertizedElements.insert(element);
}

void VSCSPropertyListener::removePropertizedElement(VSCSoundPropertizedElement* element) {
    _propertizedElements.erase(element);
}

std::set<VSCSoundPropertizedElement*> VSCSPropertyListener::getPropertizedElements(void) {
    return _propertizedElements;
}


void VSCSPropertyListener::addPropertyKey(VSCSProperty::Key k) {
    _propertyKeys.insert(k);
}

void VSCSPropertyListener::removePropertyKey(VSCSProperty::Key k) {
    _propertyKeys.erase(k);
}

std::set<VSCSProperty::Key> VSCSPropertyListener::getPropertyKeys(void) {
    return _propertyKeys;
}


void VSCSPropertyListener::setMinimumRate(VSCSFloat r) {
    _minimumRate = r;
}

VSCSFloat VSCSPropertyListener::getMinimumRate(void) {
    return _minimumRate;
}

#pragma mark - VSCSoundBroadcaster

void VSCSoundBroadcaster::parameterChanged(VSCSoundParameterizedElement* element, VSCSParameter::Key, double value) {
    
}

void VSCSoundBroadcaster::propertyChanged(VSCSoundPropertizedElement* element, VSCSProperty::Key, std::string value) {
    
}

VSCSoundBroadcastAppleRelay& VSCSoundBroadcaster::getAppleRelay(void) {
    return _appleRelay;
}


