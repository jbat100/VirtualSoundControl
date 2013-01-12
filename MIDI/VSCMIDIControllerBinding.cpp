/*
 *  VSCBoost.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDIControllerBinding.h"
#include "VSCException.h"

#include <iostream>

VSC::MIDI::ControllerBinding::ControllerBinding(void) {
    
    mMessageGenerator = MessageGenerator::SPtr(new MessageGenerator);
    
}

void VSC::MIDI::ControllerBinding::setMIDIChannel(const unsigned int chan) {
    mMIDIChannel = chan;
}

unsigned int VSC::MIDI::ControllerBinding::getMIDIChannel(void) const {
    return mMIDIChannel;
}

void VSC::MIDI::ControllerBinding::setControlNumber(const ControlNumber chan) {
    mControlNumber = chan;
}

VSC::MIDI::ControlNumber VSC::MIDI::ControllerBinding::getControlNumber(void) const {
    return mControlNumber;
}


bool VSC::MIDI::ControllerBinding::sendCurrentValue(void) {
    
    if (!mMIDIOutput) {
        std::cerr << "Attempting to send controller value with NULL mMIDIOutput " << __FILE__ << " " << __LINE__ << std::endl;
        return false;
    }
    
    if (!mController) {
        return false;
    }
    
    if (mController->getState() != VSC::Controller::StateRunning) return false;
    
    VSC::Float value = mController->getCurrentControlValue();
    
    unsigned int midiValue = (unsigned int)value;
    
    Message message = mMessageGenerator->messageForControlChange(mMIDIChannel, mControlNumber, midiValue);
    
    mMIDIOutput->sendMessage(message);
    
    return true;
    
}


