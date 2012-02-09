/*
 *  VSCBoost.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_MIDI_CONTROL_CENTER_H_
#define _VSC_MIDI_CONTROL_CENTER_H_

#include <list>
#include <boost/shared_ptr.hpp>

#include "VSCMIDI.h"
#include "RtMidi.h"

typedef  boost::shared_ptr<RtMidiIn>    RtMidiInPtr;
typedef  boost::shared_ptr<RtMidiOut>   RtMidiOutPtr;

class VSCMidiControlCenter {
    
    
public:
    
    typedef std::list<>
    
    VSCMidiControlCenter(void);
    VSCMidiControlCenter& defaultCenter();
    
    void getOuputPorts(std::list<VSCMidiOutputPort>& portList);
    void getInputPorts(std::list<VSCMidiInputPort>& portList);
    
private:
    
    RtMidiInPtr     _midiIn;
    RtMidiOutPtr    _midiOut;
    
    void createMidiOut(void);
    
};

#endif