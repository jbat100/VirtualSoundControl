/*
 *  VSCSoundBroadcaster.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundBroadcaster.h"

typedef std::set<VSCSParameterListener*>::iterator ParameterListenerIter;
typedef std::set<VSCSPropertyListener*>::iterator PropertyListenerIter;

typedef std::map<VSCSParameterId, double>::iterator ParameterUpdateMapIter;
typedef std::map<VSCSPropertyId, std::string>::iterator PropertyUpdateMapIter;

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


#pragma mark - VSCSoundBroadcaster

void VSCSoundBroadcaster::parameterChanged(VSCSoundParameterizedElement* element, VSCSParameter::Key k, double val) {
    
	VSCSParameterId paramId = {element, k};
	_parameterUpdates.erase(paramId);
	_parameterUpdates.insert( std::pair<VSCSParameterId, double>(paramId, val) );
	
}

void VSCSoundBroadcaster::propertyChanged(VSCSoundPropertizedElement* element, VSCSProperty::Key k, std::string val) {
    
	VSCSPropertyId propId = {element, k};
	_propertyUpdates.erase(propId);
	_propertyUpdates.insert( std::pair<VSCSPropertyId, std::string>(propId, val) );
	
}

VSCSoundBroadcastAppleRelay& VSCSoundBroadcaster::getAppleRelay(void) {
    return _appleRelay;
}

void VSCSoundBroadcaster::setBroadcastInterval(VSCSFloat interval) {
	_broadcastInterval = interval;
}

VSCSFloat VSCSoundBroadcaster::getBroadcastInterval(void) {
	return _broadcastInterval;
}

void VSCSoundBroadcaster::addParameterListener(VSCSParameterListener* listener) {
	_parameterListeners.insert(listener);
}

void VSCSoundBroadcaster::removeParameterListener(VSCSParameterListener* listener) {
	_parameterListeners.erase(listener);
}

void VSCSoundBroadcaster::addPropertyListener(VSCSPropertyListener* listener) {
	_propertyListeners.insert(listener);
}

void VSCSoundBroadcaster::removePropertyListener(VSCSPropertyListener* listener) {
	_propertyListeners.erase(listener);
}

void VSCSoundBroadcaster::startBroadcasting(void) {
	
}

void VSCSoundBroadcaster::stopBroadcasting(void) {
	
}

void VSCSoundBroadcaster::broadcast(void) {
	
	for (ParameterUpdateMapIter mapIt = _parameterUpdates.begin(); mapIt != _parameterUpdates.end(); mapIt++) {
		for (ParameterListenerIter paramIt = _parameterListeners.begin(); paramIt != _parameterListeners.end(); paramIt++) {
			
		}
	}
	
}


