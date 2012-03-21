/*
 *  VSCBoost.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDI.h"
#include "VSCException.h"
#include <limits>
#include <assert.h>

const VSCMIDIOutputPort     VSCMIDIOutputPortVoid   = {std::numeric_limits<unsigned int>::max(), "", false};
const VSCMIDIInputPort      VSCMIDIInputPortVoid    = {std::numeric_limits<unsigned int>::max(), "", false};

std::ostream& operator<<(std::ostream& output, const VSCMIDIOutputPort& p) {
    output << "VSCMIDIOutputPort {" << p.number << "; " << p.name << "; " << (p.isVirtual ? "virtual" : "non-virtual") << "}";
    return output;
}

std::ostream& operator<<(std::ostream& output, const VSCMIDIInputPort& p) {
    output << "VSCMIDIInputPort {" << p.number << "; " << p.name << "; " << (p.isVirtual ? "virtual" : "non-virtual") << "}";
    return output;
}


bool VSCMIDIOutputPort::operator!=(const VSCMIDIOutputPort& p) const {
    return !(*this == p);
}

bool VSCMIDIOutputPort::operator==(const VSCMIDIOutputPort& p) const {
    if (p.isVirtual != isVirtual || p.number != number || p.name.compare(name) != 0) return false;
    return true;
}

bool VSCMIDIInputPort::operator!=(const VSCMIDIInputPort& p) const {
    return !(*this == p);
}

bool VSCMIDIInputPort::operator==(const VSCMIDIInputPort& p) const {
    if (p.isVirtual != isVirtual || p.number != number || p.name.compare(name) != 0) return false;
    return true;
}

VSCMIDI::VSCMIDI(void) {
    
    // RtMidiOut constructor
    try {
        _midiOut = RtMidiOutPtr(new RtMidiOut());
    }
    catch ( RtError &error ) {
        error.printMessage();
    }
    
    // RtMidiIn constructor
    try {
        _midiIn = RtMidiInPtr(new RtMidiIn());
    }
    catch (RtError &error) {
        // Handle the exception here
        error.printMessage();
    }
    
}

VSCMIDI::~VSCMIDI(void) {
    
    // no need (WE MUST NOT!) to delete the rtmidi in/out, we are using smart pointers :)
    
}

VSCMIDI::Message VSCMIDI::messageForNote(unsigned int channel, unsigned int pitch, unsigned int velocity, bool on) {
    
    if (channel > 16 || channel < 1) 
        throw VSCInvalidArgumentException("Channel must be > 0 and < 17");
    if (pitch > 127) 
        throw VSCInvalidArgumentException("Pitch must < 128");
    if (velocity > 127) 
        throw VSCInvalidArgumentException("Velocity must < 128");
    
    Message message (3, 0);
    
    unsigned int status;
    
    if (on) status = 127;
    else status = 143;
    
    status += channel;
    
    message[0] = status;
    message[1] = pitch;
    message[2] = velocity;
    
    return message;
    
}

VSCMIDI::Message VSCMIDI::messageForPolyphonicAftertouch(unsigned int channel, unsigned int pitch, unsigned int pressure) {
    
    if (channel > 16 || channel < 1) 
        throw VSCInvalidArgumentException("Channel must be > 0 and < 17");
    if (pitch > 127) 
        throw VSCInvalidArgumentException("Pitch must < 128");
    if (pressure > 127) 
        throw VSCInvalidArgumentException("Pressure must < 128");
    
    Message message (3, 0);
    
    unsigned int status = 159;
    
    status += channel;
    
    message[0] = status;
    message[1] = pitch;
    message[2] = pressure;
    
    return message;
    
}

VSCMIDI::Message VSCMIDI::messageForChannelAftertouch(unsigned int channel, unsigned int pitch, unsigned int pressure) {
    
    if (channel > 16 || channel < 1) 
        throw VSCInvalidArgumentException("Channel must be > 0 and < 17");
    if (pitch > 127) 
        throw VSCInvalidArgumentException("Pitch must < 128");
    if (pressure > 127) 
        throw VSCInvalidArgumentException("Pressure must < 128");
    
    Message message (3, 0);
    
    unsigned int status = 207;
    
    status += channel;
    
    message[0] = status;
    message[1] = pitch;
    message[2] = pressure;
    
    return message;
    
}

VSCMIDI::Message VSCMIDI::messageForControl(unsigned int channel, unsigned int control, unsigned int value) {
    
    if (channel > 16 || channel < 1) 
        throw VSCInvalidArgumentException("Channel must be > 0 and < 17");
    if (control > 127) 
        throw VSCInvalidArgumentException("Pitch must < 128");
    if (control > 127) 
        throw VSCInvalidArgumentException("Pressure must < 128");
    
    Message message (3, 0);
    
    unsigned int status = 175;
    
    status += channel;
    
    message[0] = status;
    message[1] = control;
    message[2] = value;
    
    return message;
    
}


#pragma mark - Input and Output ports 

void VSCMIDI::refreshOutputPorts(void) {
    
    assert(_midiOut);
    
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
            throw VSCMIDIException(error.getMessage());
        }
    }
    std::cout << std::endl;
    
}

void VSCMIDI::refreshInputPorts(void) {
    
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
            throw VSCMIDIException(error.getMessage());
        }
    }
    
}

const std::list<VSCMIDIOutputPort>& VSCMIDI::getOutputPorts(void) const {
    return _outputPorts;
}

const std::list<VSCMIDIInputPort>& VSCMIDI::getInputPorts(void) const {
    return _inputPorts;
}


