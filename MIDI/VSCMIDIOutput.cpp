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

VSCMIDIOutput::VSCMIDIOutput(void) {
    
    this->createRtMidiOut();
    
}

VSCMIDIOutput::VSCMIDIOutput(VSCMIDIOutputPort outputPort) {
    
    this->createRtMidiOut();
    
    this->setOutputPort(outputPort); // will throw if does not succeed
    
}

void VSCMIDIOutput::createRtMidiOut(void) {
    
    // RtMidiOut constructor
    try {
        _midiOut = RtMidiOutPtr(new RtMidiOut());
    }
    catch ( RtError &error ) {
        throw VSCMIDIException(error.getMessage());
    }
    
}


VSCMIDIOutputPort VSCMIDIOutput::getOutputPort(void) const {
    return _outputPort;
}

void VSCMIDIOutput::setOutputPort(VSCMIDIOutputPort const& port) {
    
    if (port != VSCMIDIOutputPortVoid && _midiOut) {
        
        _midiOut->closePort();
        
        if (port.isVirtual) {
            try {
                _midiOut->openVirtualPort(port.name);
            } catch (RtError &error) {
                error.printMessage();
                _outputPort = VSCMIDIOutputPortVoid;
                throw VSCMIDIException(error.getMessage());
            }
            _outputPort = port;
            std::cout << "Successfully opened " << port << std::endl;
            return;
        }
        
        else {
            try {
                if (port.name.size() > 0) _midiOut->openPort(port.number);
                else _midiOut->openPort(port.number, port.name);
            } catch (RtError &error) {
                error.printMessage();
                _outputPort = VSCMIDIOutputPortVoid;
                throw VSCMIDIException(error.getMessage());
            }
            _outputPort = port;
            std::cout << "Successfully opened " << port << std::endl;
        }
        
    }
    
    else {
        
        if (_midiOut) {
            _outputPort = VSCMIDIOutputPortVoid;
            _midiOut->closePort();
        }
        
        else {
            _outputPort = VSCMIDIOutputPortVoid;
            throw VSCMIDIException("Expected non nil _midiOut");
        }
    }
    
    
}

bool VSCMIDIOutput::sendMessage(VSCMIDI::Message& m) {
    
    if (_midiOut && _outputPort != VSCMIDIOutputPortVoid) {
        std::cout << *this << " sending " << VSCMIDI::messageDescription(m) << std::endl;
        _midiOut->sendMessage(&m);
    }
    
    return false;
    
}

std::ostream& operator<<(std::ostream& output, const VSCMIDIOutput& p) {
    output << "VSCMIDIOutput (" << p.getOutputPort() << ")";
    return output;
}
