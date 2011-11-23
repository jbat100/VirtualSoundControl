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

bool VSCSParameterListener::interestedInParameterId(VSCSParameterId paramId) {
    
    if (_parameterKeys.find(paramId.key) == _parameterKeys.end()) 
        return false;
    
    if (_parameterizedElements.find(paramId.element) == _parameterizedElements.end())
        return false;
    
    return true;
    
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

bool VSCSPropertyListener::interestedInPropertyId(VSCSPropertyId propId) {
    
    if (_propertyKeys.find(propId.key) == _propertyKeys.end()) 
        return false;
    
    if (_propertizedElements.find(propId.element) == _propertizedElements.end())
        return false;
    
    return true;
    
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
    
    for (PropertyUpdateMapIter mapIt = _propertyUpdates.begin(); mapIt != _propertyUpdates.end(); mapIt++) {
        
        std::pair<VSCSPropertyId, std::string> propUpdate = *mapIt;
        VSCSPropertyId mapPropId = propUpdate.first;
        std::string val = propUpdate.second;
        
		for (PropertyListenerIter propIt = _propertyListeners.begin(); propIt != _propertyListeners.end(); propIt++) {
			VSCSPropertyListener* listener = *propIt;
            if (listener->interestedInPropertyId(mapPropId)) {
                listener->propertyChanged(mapPropId.element, mapPropId.key, val);
            }
		}
        
#ifdef __APPLE__
        _appleRelay.propertyChanged(mapPropId.element, mapPropId.key, val);
#endif
        
	}
	
	for (ParameterUpdateMapIter mapIt = _parameterUpdates.begin(); mapIt != _parameterUpdates.end(); mapIt++) {
        
        std::pair<VSCSParameterId, double> paramUpdate = *mapIt;
        VSCSParameterId mapParamId = paramUpdate.first;
        double val = paramUpdate.second;
        
		for (ParameterListenerIter paramIt = _parameterListeners.begin(); paramIt != _parameterListeners.end(); paramIt++) {
			VSCSParameterListener* listener = *paramIt;
            if (listener->interestedInParameterId(mapParamId)) {
                listener->parameterChanged(mapParamId.element, mapParamId.key, val);
            }
		}
        
#ifdef __APPLE__
        _appleRelay.parameterChanged(mapParamId.element, mapParamId.key, val);
#endif
        
	}
    
    
	
}


