/*
 *  VSCBoost.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDIController.h"

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

void VSCMIDIController::sendMIDIStateIfRequired(void) {
    
}

