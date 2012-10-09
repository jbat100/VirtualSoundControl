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


#pragma mark - Constructor/Destructor

VSC::MIDI::ControlCenter::ControlCenter(void) {
    

    
}

VSC::MIDI::ControlCenter::~ControlCenter(void) {
    
    // using smart pointer for _midiOut so no need to delete
    
}

/*

boost::mutex VSCMIDIControlCenter::_singletonMutex;

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

 */

#pragma mark - MIDI Controllers 

void VSC::MIDI::ControlCenter::addControllerBinding(ControllerBinding::SPtr controller) {
    ControllerBindings::iterator it = std::find(mControllerBindings.begin(), mControllerBindings.end(), controller);
    if (it == mControllerBindings.end()) {
        mControllerBindings.push_back(controller);
    }
}

void VSC::MIDI::ControlCenter::removeControllerBinding(ControllerBinding::SPtr controller) {
    ControllerBindings::iterator it = std::find(mControllerBindings.begin(), mControllerBindings.end(), controller);
    if (it != mControllerBindings.end()) {
        mControllerBindings.erase(it);
    }
}

const VSC::MIDI::ControllerBindings& VSC::MIDI::ControlCenter::getControllerBindings(void) const {
    return mControllerBindings;
}

#pragma mark - Broadcasting 

VSC::Sound::TimeInterval VSC::MIDI::ControlCenter::getBroadcastInterval(void) const {
    return mBroadcastInterval;
}

void VSC::MIDI::ControlCenter::setBroadcastInterval(const VSC::Sound::TimeInterval  interval) {
    mBroadcastInterval = interval;
}

void VSC::MIDI::ControlCenter::broadcastNow(void) {
    
}

void VSC::MIDI::ControlCenter::startPeriodicBroadcasting(void) {
    
    // use boost asio !!
    
}

void VSC::MIDI::ControlCenter::stopPeriodicBroadcasting(void) {
    
}


