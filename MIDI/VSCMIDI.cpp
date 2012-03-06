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


bool VSCMIDIOutputPort::operator!=(const VSCMIDIOutputPort& p) {
    return !(*this == p);
}

bool VSCMIDIOutputPort::operator==(const VSCMIDIOutputPort& p) {
    if (p.isVirtual != isVirtual || p.number != number || p.name.compare(name) != 0) return false;
    return true;
}

bool VSCMIDIInputPort::operator!=(const VSCMIDIInputPort& p) {
    return !(*this == p);
}

bool VSCMIDIInputPort::operator==(const VSCMIDIInputPort& p) {
    if (p.isVirtual != isVirtual || p.number != number || p.name.compare(name) != 0) return false;
    return true;
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


