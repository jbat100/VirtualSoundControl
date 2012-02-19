/*
 *  VSCBoost.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDIController.h"

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

void VSCMIDIController::setChannel(unsigned int chan) {
    
}

unsigned int VSCMIDIController::getChannel(void) {
    
}

void VSCMIDIController::setNormalizeToMIDIRange(bool norm) {
    _normalizeToMIDIRange = norm;
}

bool VSCMIDIController::getNormalizeToMIDIRange(void) {
    return _normalizeToMIDIRange;
}

VSCMIDIOutputPort VSCMIDIController::getOutputPort(void) {
    return _outputPort;
}

void VSCMIDIController::setOutputPort(VSCMIDIOutputPort port) {
    
    if (port != VSCMIDIOutputPortVoid && _midiOut) {
        
        _midiOut->closePort();
        
        if (port.isVirtual) {
            try {
                _midiOut->openVirtualPort(port.name);
            } catch (RtError &error) {
                error.printMessage();
                _outputPort = VSCMIDIOutputPortVoid;
                return;
            }
            _outputPort = port;
            std::cout << "Successfully opened " << port << std::endl;
        }
        
        else {
            try {
                if (port.name.size() > 0) _midiOut->openPort(port.number);
                else _midiOut->openPort(port.number, port.name);
            } catch (RtError &error) {
                error.printMessage();
                _outputPort = VSCMIDIOutputPortVoid;
                return;
            }
            _outputPort = port;
        }
        
    }
    
    else {
        _outputPort = VSCMIDIOutputPortVoid;
    }
    
}

void VSCMIDIController::addMidiControlIdentifier(unsigned int identifier) {
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
    if (controller->getState() != VSCController::StateRunning) return;
    
    VSCSFloat value = _controller->getCurrentControlValue();
    
    unsigned int midiValue;
    
    if (_normalizeToMIDIRange) {
        VSCController::ValueRange range = _controller->getValueRange();
        VSCSFloat mult = 127.0 / range.second();
        VSCSFloat result = range.first() + (value*mult);
        midiValue = (unsigned int)result;
    }
    else {
        midiValue = (unsigned int)value;
    }
    
    for (ControlIdentifiers::iterator it = _midiControlIdentifiers.begin(); it != _midiControlIdentifiers.end(); it++) {
        unsigned int controlIdentifier = *it;
        VSCMIDI::Message message = VSCMIDI::messageForControl(<#unsigned int channel#>, <#unsigned int control#>, <#unsigned int value#>)
        _midiOut->sendMessage(<#std::vector<unsigned char> *message#>)
    }
    
}

