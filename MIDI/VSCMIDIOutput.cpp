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

#include "RtError.h"

#include <iostream>

VSC::MIDI::Output::Output(void) {
    
    this->createRtMidiOut();
    
}

VSC::MIDI::Output::Output(OutputPort outputPort) {
    
    this->createRtMidiOut();
    
    this->setOutputPort(outputPort); // will throw if does not succeed
    
}

void VSC::MIDI::Output::createRtMidiOut(void) {
    
    // RtMidiOut constructor
    try {
        mMIDIOut = RtMidiOutPtr(new RtMidiOut());
    }
    catch ( RtError &error ) {
        throw VSCMIDIException(error.getMessage());
    }
    
}


VSC::MIDI::OutputPort VSC::MIDI::Output::getOutputPort(void) const {
    return mOutputPort;
}

void VSC::MIDI::Output::setOutputPort(OutputPort const& port) {
    
    if (port != OutputPortVoid && mMIDIOut) {
        
        mMIDIOut->closePort();
        
        if (port.isVirtual) {
            try {
                mMIDIOut->openVirtualPort(port.name);
            } catch (RtError &error) {
                error.printMessage();
                mOutputPort = OutputPortVoid;
                throw VSCMIDIException(error.getMessage());
            }
            mOutputPort = port;
            std::cout << "Successfully opened " << port << std::endl;
            return;
        }
        
        else {
            try {
                if (port.name.size() > 0) mMIDIOut->openPort(port.number);
                else mMIDIOut->openPort(port.number, port.name);
            } catch (RtError &error) {
                error.printMessage();
                mOutputPort = OutputPortVoid;
                throw VSCMIDIException(error.getMessage());
            }
            mOutputPort = port;
            std::cout << "Successfully opened " << port << std::endl;
        }
        
    }
    
    else {
        
        if (mMIDIOut) {
            mOutputPort = OutputPortVoid;
            mMIDIOut->closePort();
        }
        
        else {
            mOutputPort = OutputPortVoid;
            throw VSCMIDIException("Expected non nil mMIDIOut");
        }
    }
    
    
}

bool VSC::MIDI::Output::sendMessage(VSCMIDI::Message& m) {
    
    if (mMIDIOut && mOutputPort != OutputPortVoid) {
        std::cout << *this << " sending " << VSCMIDI::messageDescription(m) << std::endl;
        mMIDIOut->sendMessage(&m);
    }
    
    return false;
    
}

std::ostream& VSC::MIDI::operator<<(std::ostream& output, const Output& p) {
    output << "VSCMIDIOutput (" << p.getOutputPort() << ")";
    return output;
}
