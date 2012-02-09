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
    
}


void VSCMidiControlCenter::createMidiOut(void) {

    // RtMidiOut constructor
    try {
        _midiOut = RtMidiOutPtr(new RtMidiOut());
    }
    catch ( RtError &error ) {
        error.printMessage();
        exit( EXIT_FAILURE );
    }

}

void VSCMidiControlCenter::getOuputPorts(std::list<VSCMidiOutputPort>& portList) {
    
    // Check outputs.
    unsigned int nPorts = _midiOut->getPortCount();
    std::string portName;
    std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = _midiOut->getPortName(i);
            VSCMidiOutputPort p = {i, portName};
            portList.push_back(p);
            std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
        }
        catch (RtError &error) {
            error.printMessage();
        }
    }
    std::cout << std::endl;
    
}

void VSCMidiControlCenter::getInputPorts(std::list<VSCMidiInputPort>& portList) {
    
    // Check inputs.
    unsigned int nPorts = _midiIn->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = _midiIn->getPortName(i);
            std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
        }
        catch ( RtError &error ) {
            error.printMessage();
        }
    }
    
}



