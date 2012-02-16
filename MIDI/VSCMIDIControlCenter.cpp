/*
 *  VSCBoost.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDIControlCenter.h"
//#include "VSCBoost.h"

VSCMidiControlCenter::VSCMidiControlCenter(void) {
    
    // RtMidiOut constructor
    try {
        _midiOut = RtMidiOutPtr(new RtMidiOut());
    }
    catch ( RtError &error ) {
        error.printMessage();
        exit( EXIT_FAILURE );
    }
    
}

void VSCMidiControlCenter::refreshOutputPorts(void) {
    
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

void VSCMidiControlCenter::refreshInputPorts(void) {
    
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

const std::list<VSCMIDIOutputPort>& VSCMidiControlCenter::getOuputPorts(void) {
    return _outputPorts;
}

const std::list<VSCMIDIInputPort>& VSCMidiControlCenter::getInputPorts(void) {
    return _inputPorts;
}



