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

VSCMIDIOutput::VSCMIDIOutput(void) {
    
    // RtMidiOut constructor
    try {
        _midiOut = RtMidiOutPtr(new RtMidiOut());
    }
    catch ( RtError &error ) {
        error.printMessage();
        exit( EXIT_FAILURE );
    }
    
}

VSCMIDIOutput::VSCMIDIOutput(VSCMIDIOutputPort outputPort) {
    
    bool success = this->setOutputPort(outputPort);
    
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
                throw VSCMIDIException(error.message());
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
                throw VSCMIDIException(error.message());
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
            throw VSCMIDIException("Expected non nil _midiOut");
        }
    }
    
    return false;
    
}

bool VSCMIDIOutput::sendMessage(VSCMIDI::Message const& m) {
    
    if (_midiOut && _outputPort != VSCMIDIOutputPortVoid) {
        _midiOut->sendMessage(m);
    }
    
    return false;
    
}

