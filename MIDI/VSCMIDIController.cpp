/*
 *  VSCBoost.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDIController.h"
#include "VSCException.h"

#include <iostream>

VSCMIDIController::VSCMIDIController(void) {
    
    // RtMidiOut constructor
    try {
        _midiOut = RtMidiOutPtr(new RtMidiOut());
    }
    catch ( RtError &error ) {
        error.printMessage();
        exit( EXIT_FAILURE );
    }
    
}

VSCMIDIController::VSCMIDIController(VSCMIDIOutputPort outputPort) {
    
    bool success = this->setOutputPort(outputPort);
    
    if (success == false) {
        // Throwing an exception makes sure we do not leak memory http://www.parashift.com/c++-faq-lite/exceptions.html#faq-17.8 
        std::stringstream errStream;
        errStream << "Could not open " << outputPort;
        throw VSCMIDIException(errStream.str());
    }
    
}

void VSCMIDIController::setChannel(unsigned int chan) {
    _channel = chan;
}

unsigned int VSCMIDIController::getChannel(void) const {
    return _channel;
}

void VSCMIDIController::setNormalizeToMIDIRange(bool norm) {
    _normalizeToMIDIRange = norm;
}

bool VSCMIDIController::getNormalizeToMIDIRange(void) {
    return _normalizeToMIDIRange;
}

VSCMIDIOutputPort VSCMIDIController::getOutputPort(void) const {
    return _outputPort;
}

bool VSCMIDIController::setOutputPort(VSCMIDIOutputPort port) {
    
    if (port != VSCMIDIOutputPortVoid && _midiOut) {
        
        _midiOut->closePort();
        
        if (port.isVirtual) {
            try {
                _midiOut->openVirtualPort(port.name);
            } catch (RtError &error) {
                error.printMessage();
                _outputPort = VSCMIDIOutputPortVoid;
                return false;
            }
            _outputPort = port;
            std::cout << "Successfully opened " << port << std::endl;
            return true;
        }
        
        else {
            try {
                if (port.name.size() > 0) _midiOut->openPort(port.number);
                else _midiOut->openPort(port.number, port.name);
            } catch (RtError &error) {
                error.printMessage();
                _outputPort = VSCMIDIOutputPortVoid;
                return false;
            }
            _outputPort = port;
            std::cout << "Successfully opened " << port << std::endl;
            return true;
        }
        
    }
    
    else {
        
        if (_midiOut) {
            _outputPort = VSCMIDIOutputPortVoid;
            _midiOut->closePort();
            return true;
        }
        
        else {
            _outputPort = VSCMIDIOutputPortVoid;
            return false;
        }
    }
    
    return false;
    
}

void VSCMIDIController::addControlIdentifier(unsigned int identifier) {
    _midiControlIdentifiers.insert(identifier);
}

void VSCMIDIController::removeContorlIdentifier(unsigned int identifier) {
    _midiControlIdentifiers.erase(identifier);
}

const VSCMIDIController::ControlIdentifiers& VSCMIDIController::getControlIdentifiers(void) {
    return _midiControlIdentifiers;
}

void VSCMIDIController::sendMIDIStateIfRequired(void) {
    
    if (!_midiOut) return;
    if (!_controller) return;
    if (_controller->getState() != VSCController::StateRunning) return;
    
    VSCSFloat value = _controller->getCurrentControlValue();
    
    unsigned int midiValue;
    
    if (_normalizeToMIDIRange) {
        VSCController::ValueRange range = _controller->getValueRange();
        VSCSFloat mult = 127.0 / range.second;
        VSCSFloat result = range.first + (value*mult);
        midiValue = (unsigned int)result;
    }
    else {
        midiValue = (unsigned int)value;
    }
    
    for (ControlIdentifiers::iterator it = _midiControlIdentifiers.begin(); it != _midiControlIdentifiers.end(); it++) {
        unsigned int controlIdentifier = *it;
        VSCMIDI::Message message = VSCMIDI::messageForControl(_channel, controlIdentifier, midiValue);
        _midiOut->sendMessage(&message);
    }
    
}

