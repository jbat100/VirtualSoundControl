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


typedef std::set<VSCSBroadcastListenerPtr>::iterator BroadcastListenerIter;
typedef std::map<VSCSParameterId, double>::iterator ParameterUpdateMapIter;
typedef std::map<VSCSIndexedParameterId, double>::iterator IndexedParameterUpdateMapIter;
typedef std::map<VSCSPropertyId, std::string>::iterator PropertyUpdateMapIter;

bool VSCSParameterId::operator<(const VSCSParameterId& paramId) const { 
	if (element < paramId.element)
		return true; 
	if (key < paramId.key)
		return true; 
	return false;
}

bool VSCSIndexedParameterId::operator<(const VSCSIndexedParameterId& paramId) const { 
	if (element < paramId.element)
		return true; 
	if (key < paramId.key)
		return true; 
	if (index < paramId.index)
		return true;
	return false;
}

#pragma mark - VSCSBroadcastListener

void VSCSBroadcastListener::parameterChanged(VSCSParameterId id, double value) {
	std::cout << "PARAMETER CHANGED" << std::endl;
}

void VSCSBroadcastListener::indexedParameterChanged(VSCSIndexedParameterId id, double value) {
	std::cout << "PARAMETER CHANGED" << std::endl;
}

void VSCSBroadcastListener::propertyChanged(VSCSPropertyId, std::string value) {
	std::cout << "PROPERTY CHANGED" << std::endl;
}


#pragma mark - VSCSoundBroadcaster

VSCSoundBroadcaster::VSCSoundBroadcaster() {
	
}

VSCSoundBroadcaster::~VSCSoundBroadcaster() {
	
}

void VSCSoundBroadcaster::parameterChanged(VSCSParameterId, double val) {
	boost::mutex::scoped_lock updateLock(updateMutex);
	_parameterUpdates.erase(paramId);
	_parameterUpdates.insert( std::pair<VSCSParameterId, double>(paramId, val) );
}

void VSCSoundBroadcaster::indexedParameterChanged(VSCSIndexedParameterId, double value) {
	boost::mutex::scoped_lock updateLock(updateMutex);
	_indexedParameterUpdateMap.erase(paramId);
	_indexedParameterUpdateMap.insert( std::pair<VSCSIndexedParameterId, double>(paramId, val) );
}

void VSCSoundBroadcaster::propertyChanged(VSCSPropertyId propId, std::string val) {
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

void VSCSoundBroadcaster::addBroadcastListener(VSCSBroadcastListenerPtr listener) {
	boost::mutex::scoped_lock listenerLock(listenerMutex);
	_broadcastListeners.insert(listener);
}

void VSCSoundBroadcaster::removeBroadcastListener(VSCSBroadcastListenerPtr listener) {
	boost::mutex::scoped_lock listenerLock(listenerMutex);
	_broadcastListeners.erase(listener);
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
	std::set<VSCSBroadcastListenerPtr> listenersCopy = _broadcastListeners;
	listenerMutex.unlock();
	
	updateMutex.lock();
	std::map<VSCSPropertyId, std::string> propUpdatesCopy = _propertyUpdates;
	std::map<VSCSParameterId, double> paramUpdatesCopy = _parameterUpdates;
	updateMutex.unlock();
	
	for (BroadcastListenerIter lIt = paramListenersCopy.begin(); lIt != paramListenersCopy.end(); lIt++) {
		
		VSCSBroadcastListenerPtr* listener = *lIt;
		
		for (ParameterUpdateMapIter it = paramUpdatesCopy.begin(); it != paramUpdatesCopy.end(); it++) {
			std::pair<VSCSParameterId, double> paramUpdate = *it;
			VSCSParameterId mapParamId = paramUpdate.first;
			double val = paramUpdate.second;
			listener->parameterChanged(VSCSParameterId, val);
		}
		
		for (IndexedParameterUpdateMapIter it = paramUpdatesCopy.begin(); it != paramUpdatesCopy.end(); it++) {
			std::pair<VSCSIndexedParameterId, double> paramUpdate = *it;
			VSCSIndexedParameterId mapParamId = paramUpdate.first;
			double val = paramUpdate.second;
			listener->indexedParameterChanged(VSCSIndexedParameterId, val);
		}
	
		for (PropertyUpdateMapIter it = propUpdatesCopy.begin(); it != propUpdatesCopy.end(); it++) {
			std::pair<VSCSPropertyId, std::string> propUpdate = *it;
			VSCSPropertyId mapPropId = propUpdate.first;
			std::string val = propUpdate.second;
			listener->propertyChanged(VSCSPropertyId, val);
		}
	
	}
    


	
}


