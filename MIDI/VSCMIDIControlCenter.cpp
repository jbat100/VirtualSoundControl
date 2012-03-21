/*
 *  VSCBoost.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDIControlCenter.h"
#include "VSCException.h"
//#include "VSCBoost.h"

#include "RtError.h"

boost::mutex VSCMIDIControlCenter::_singletonMutex;

#pragma mark - Constructor/Destructor

VSCMIDIControlCenter::VSCMIDIControlCenter(void) {
    

    
}

VSCMIDIControlCenter::~VSCMIDIControlCenter(void) {
    
    // using smart pointer for _midiOut so no need to delete
    
}

VSCMIDIControlCenterPtr VSCMIDIControlCenter::defaultCenter(void) {
    
    static VSCMIDIControlCenterPtr sharedInstancePtr = VSCMIDIControlCenterPtr();
    
    if (!sharedInstancePtr) {
        boost::mutex::scoped_lock lock(_singletonMutex);
        if (!sharedInstancePtr) {
            sharedInstancePtr = VSCMIDIControlCenterPtr(new VSCMIDIControlCenter());
        }
    }
    
    return sharedInstancePtr;
    
}


#pragma mark - MIDI Controllers 

void VSCMIDIControlCenter::addController(VSCMIDIControllerPtr controller) {
    ControllerList::iterator it = std::find(_controllerList.begin(), _controllerList.end(), controller);
    if (it == _controllerList.end()) {
        _controllerList.push_back(controller);
    }
}

void VSCMIDIControlCenter::removeController(VSCMIDIControllerPtr controller) {
    ControllerList::iterator it = std::find(_controllerList.begin(), _controllerList.end(), controller);
    if (it != _controllerList.end()) {
        _controllerList.erase(it);
    }
}

const VSCMIDIControlCenter::ControllerList& VSCMIDIControlCenter::getControllerList(void) const {
    return _controllerList;
}

#pragma mark - Broadcasting 

VSCSTimeInterval VSCMIDIControlCenter::getBroadcastInterval(void) const {
    return _broadcastInterval;
}

void VSCMIDIControlCenter::setBroadcastInterval(const VSCSTimeInterval interval) {
    _broadcastInterval = interval;
}

void VSCMIDIControlCenter::broadcastNow(void) {
    
}

void VSCMIDIControlCenter::startPeriodicBroadcasting(void) {
    
}

void VSCMIDIControlCenter::stopPeriodicBroadcasting(void) {
    
}


