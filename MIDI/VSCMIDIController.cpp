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
    

    
}

void VSCMIDIController::setMIDIChannel(const unsigned int chan) {
    _midiChannel = chan;
}

unsigned int VSCMIDIController::getMIDIChannel(void) const {
    return _midiChannel;
}

void VSCMIDIController::setControlChannel(const unsigned int chan) {
    _controlChannel = chan;
}

unsigned int VSCMIDIController::getControlChannel(void) const {
    return _controlChannel;
}


bool VSCMIDIController::sendControllerValue(void) {
    
    if (!_midiOutput) {
        std::cerr << "Attempting to send controller value with NULL _midiOutput " << __FILE__ << " " << __LINE__ << std::endl;
        return false;
    }
    
    if (!_controller) {
        return false;
    }
    
    if (_controller->getState() != VSCController::StateRunning) return false;
    
    VSCSFloat value = _controller->getCurrentControlValue();
    
    unsigned int midiValue;
    
    if (1) {
        VSCController::ValueRange range = _controller->getValueRange();
        VSCSFloat mult = 127.0 / range.second;
        VSCSFloat result = range.first + (value*mult);
        midiValue = (unsigned int)result;
    }
    else {
        midiValue = (unsigned int)value;
    }
    
    VSCMIDI::Message message = VSCMIDI::messageForControl(_midiChannel, _controlChannel, midiValue);
    _midiOutput->sendMessage(message);
    
    return true;
    
}

bool VSCMIDIController::sendValue(unsigned int val) {
    
    if (!_midiOutput) {
        std::cerr << "Attempting to send value with NULL _midiOutput " << __FILE__ << " " << __LINE__ << std::endl;
        return false;
    }
    
    VSCMIDI::Message message = VSCMIDI::messageForControl(_midiChannel, _controlChannel, val);
    _midiOutput->sendMessage(message);
    
    return true;
    
}

