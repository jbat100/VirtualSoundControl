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
#include "VSCMIDIController.h"
#include "RtMidi.h"


class VSCMIDIControlCenter {
    
    
public:
    
    typedef std::list<VSCMIDIControllerPtr> ControllerList;
    
    VSCMIDIControlCenter(void);
    VSCMIDIControlCenter& defaultCenter();
    
    void refreshInputPorts(void);
    void refreshOutputPorts(void);
    
    const std::list<VSCMIDIOutputPort>& getOuputPorts(void) const;
    const std::list<VSCMIDIInputPort>& getInputPorts(void) const;
    
    void addController(VSCMIDIControllerPtr controller);
    void removeController(VSCMIDIControllerPtr controller);
    const ControllerList& getControllerList(void) const;
    
private:
    
    RtMidiInPtr     _midiIn;
    RtMidiOutPtr    _midiOut;
    
    std::list<VSCMIDIInputPort> _inputPorts;
    std::list<VSCMIDIOutputPort> _outputPorts;
    
    ControllerList _controllerList;
    
};

#endif