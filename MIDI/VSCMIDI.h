/*
 *  VSCBoost.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_MIDI_H_
#define _VSC_MIDI_H_

#include <list>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include "RtMidi.h"

typedef  boost::shared_ptr<RtMidiIn>    RtMidiInPtr;
typedef  boost::shared_ptr<RtMidiOut>   RtMidiOutPtr;

struct VSCMIDIOutputPort {
    unsigned int number;
    std::string name;
    bool isVirtual;
    bool operator!=(const VSCMIDIOutputPort& p);
    bool operator==(const VSCMIDIOutputPort& p);
};

struct VSCMIDIInputPort {
    unsigned int number;
    std::string name;
    bool isVirtual;
    bool operator!=(const VSCMIDIInputPort& p);
    bool operator==(const VSCMIDIInputPort& p);
};

extern const VSCMIDIOutputPort VSCMIDIOutputPortVoid;
extern const VSCMIDIInputPort VSCMIDIInputPortVoid;

class VSCMIDI {
    
    typedef std::vector<unsigned char> Message;
    
public:
    
    static Message messageForNote(unsigned int channel, unsigned int pitch, bool on);
    
};

#endif
