/*
 *  VSCSoundBroadcaster.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundBroadcaster.h"

#include <iostream>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


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
    
    if (paramId.key != VSCSParameter::KeyAll && _parameterKeys.find(paramId.key) == _parameterKeys.end()) 
        return false;
    
    if (_parameterizedElements.find(paramId.element) == _parameterizedElements.end())
        return false;
    
    return true;
    
}

void VSCSParameterListener::parameterChanged(VSCSoundParameterizedElement* element, VSCSParameter::Key, double value) {
	std::cout << "PARAMETER CHANGED" << std::endl;
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
    
    if (propId.key != VSCSProperty::KeyAll && _propertyKeys.find(propId.key) == _propertyKeys.end()) 
        return false;
    
    if (_propertizedElements.find(propId.element) == _propertizedElements.end())
        return false;
    
    return true;
    
}

void VSCSPropertyListener::propertyChanged(VSCSoundPropertizedElement* element, VSCSProperty::Key, std::string value) {
	std::cout << "PROPERTY CHANGED" << std::endl;
}


#pragma mark - VSCSoundBroadcaster

VSCSoundBroadcaster::VSCSoundBroadcaster() {
	
}

VSCSoundBroadcaster::~VSCSoundBroadcaster() {
	
}

void VSCSoundBroadcaster::parameterChanged(VSCSoundParameterizedElement* element, VSCSParameter::Key k, double val) {
    
	VSCSParameterId paramId = {element, k};
	boost::mutex::scoped_lock updateLock(updateMutex);
	_parameterUpdates.erase(paramId);
	_parameterUpdates.insert( std::pair<VSCSParameterId, double>(paramId, val) );
	
}

void VSCSoundBroadcaster::propertyChanged(VSCSoundPropertizedElement* element, VSCSProperty::Key k, std::string val) {
    
	VSCSPropertyId propId = {element, k};
	boost::mutex::scoped_lock updateLock(updateMutex);
	_propertyUpdates.erase(propId);
	_propertyUpdates.insert( std::pair<VSCSPropertyId, std::string>(propId, val) );
	
}

void VSCSoundBroadcaster::setBroadcastInterval(VSCSFloat interval) {
	boost::mutex::scoped_lock intervalLock(intervalMutex);
	_broadcastInterval = interval;
}

VSCSFloat VSCSoundBroadcaster::getBroadcastInterval(void) {
	boost::mutex::scoped_lock intervalLock(intervalMutex);
	return _broadcastInterval;
}

void VSCSoundBroadcaster::addParameterListener(VSCSParameterListener* listener) {
	boost::mutex::scoped_lock listenerLock(listenerMutex);
	_parameterListeners.insert(listener);
}

void VSCSoundBroadcaster::removeParameterListener(VSCSParameterListener* listener) {
	boost::mutex::scoped_lock listenerLock(listenerMutex);
	_parameterListeners.erase(listener);
}

void VSCSoundBroadcaster::addPropertyListener(VSCSPropertyListener* listener) {
	boost::mutex::scoped_lock listenerLock(listenerMutex);
	_propertyListeners.insert(listener);
}

void VSCSoundBroadcaster::removePropertyListener(VSCSPropertyListener* listener) {
	boost::mutex::scoped_lock listenerLock(listenerMutex);
	_propertyListeners.erase(listener);
}

void VSCSoundBroadcaster::startBroadcasting(void) {
	
	broadcastCycleThread.interrupt();
	
	broadcastCycleThread = boost::thread(boost::bind(&VSCSoundBroadcaster::cycleBroadcastAndSleepUntilStopped, this)); 
	
}

void VSCSoundBroadcaster::stopBroadcasting(void) {
	
	broadcastCycleThread.interrupt();
	
}


/*
 *	This method is meant to be called only with boost::thread (which can be interrupted)
 */
void VSCSoundBroadcaster::cycleBroadcastAndSleepUntilStopped(void) {
	
	while (1) {
		this->broadcast();
		VSCSFloat interval = this->getBroadcastInterval();
		boost::posix_time::time_duration t = boost::posix_time::milliseconds(int(interval*1000.0));  
		boost::this_thread::sleep(t); // sleep is an interrupt point so the thread can be terminated here
	}
}

void VSCSoundBroadcaster::broadcast(void) {
	
	/*
	 *	Make copies before broadcasting to minimze lock time
	 */
	
	listenerMutex.lock();
	std::set<VSCSPropertyListener*> propListenersCopy = _propertyListeners;
	listenerMutex.unlock();
	
	updateMutex.lock();
	std::map<VSCSPropertyId, std::string> propUpdatesCopy = _propertyUpdates;
	updateMutex.unlock();
	
    
    for (PropertyUpdateMapIter mapIt = propUpdatesCopy.begin(); mapIt != propUpdatesCopy.end(); mapIt++) {
        
        std::pair<VSCSPropertyId, std::string> propUpdate = *mapIt;
        VSCSPropertyId mapPropId = propUpdate.first;
        std::string val = propUpdate.second;
        
		for (PropertyListenerIter propIt = propListenersCopy.begin(); propIt != propListenersCopy.end(); propIt++) {
			VSCSPropertyListener* listener = *propIt;
            if (listener->interestedInPropertyId(mapPropId)) {
                listener->propertyChanged(mapPropId.element, mapPropId.key, val);
            }
		}
        
	}
	
	/*
	 *	Make copies before broadcasting to minimze lock time
	 */
	
	listenerMutex.lock();
	std::set<VSCSParameterListener*> paramListenersCopy = _parameterListeners;
    listenerMutex.unlock();
	
	updateMutex.lock();
	std::map<VSCSParameterId, double> paramUpdatesCopy = _parameterUpdates;
	updateMutex.unlock();
	
	for (ParameterUpdateMapIter mapIt = paramUpdatesCopy.begin(); mapIt != paramUpdatesCopy.end(); mapIt++) {
        
        std::pair<VSCSParameterId, double> paramUpdate = *mapIt;
        VSCSParameterId mapParamId = paramUpdate.first;
        double val = paramUpdate.second;
        
		for (ParameterListenerIter paramIt = paramListenersCopy.begin(); paramIt != paramListenersCopy.end(); paramIt++) {
			VSCSParameterListener* listener = *paramIt;
            if (listener->interestedInParameterId(mapParamId)) {
                listener->parameterChanged(mapParamId.element, mapParamId.key, val);
            }
		}

	}
	
}


