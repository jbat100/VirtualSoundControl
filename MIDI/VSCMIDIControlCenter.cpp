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

VSCMIDIControlCenter::VSCMIDIControlCenter(void) {
    
    // RtMidiOut constructor
    try {
        _midiOut = RtMidiOutPtr(new RtMidiOut());
    }
    catch ( RtError &error ) {
        //error.printMessage();
        throw VSCMIDIException(error.getMessage());
        //throw VSCMIDIException("test message");
    }
    
}

VSCMIDIControlCenter::~VSCMIDIControlCenter(void) {
    
    // using smart pointer for _midiOut so no need to delete
    
}

#pragma mark - Input and Output ports 

void VSCMIDIControlCenter::refreshOutputPorts(void) {
    
    _outputPorts.clear();
    
    // Check outputs.
    unsigned int nPorts = _midiOut->getPortCount();
    std::string portName;
    std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = _midiOut->getPortName(i);
            VSCMIDIOutputPort p = {i, portName};
            _outputPorts.push_back(p);
            std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
        }
        catch (RtError &error) {
            error.printMessage();
        }
    }
    std::cout << std::endl;
    
}

void VSCMIDIControlCenter::refreshInputPorts(void) {
    
    _inputPorts.clear();
    
    // Check inputs.
    unsigned int nPorts = _midiIn->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = _midiIn->getPortName(i);
            VSCMIDIInputPort p = {i, portName};
            _inputPorts.push_back(p);
            std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
        }
        catch ( RtError &error ) {
            error.printMessage();
        }
    }
    
}

const std::list<VSCMIDIOutputPort>& VSCMIDIControlCenter::getOuputPorts(void) const {
    return _outputPorts;
}

const std::list<VSCMIDIInputPort>& VSCMIDIControlCenter::getInputPorts(void) const {
    return _inputPorts;
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


