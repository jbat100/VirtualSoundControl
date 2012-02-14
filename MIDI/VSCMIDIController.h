/*
 *  VSCBoost.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_MIDI_CONTROLLER_H_
#define _VSC_MIDI_CONTROLLER_H_

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "VSCSound.h"
#include "VSCMIDI.h"
#include "VSCController.h"

#include "RtMidi.h"

/*
 *  A detailed table of midi messages can be found here http://www.midi.org/techspecs/midimessages.php
 *
 *  A bit of background information from wiki of MIDI messages http://en.wikipedia.org/wiki/MIDI_1.0
 * 
 */


class VSCMIDIController {
    
public:
    
    VSCMIDIController(void);
    
    VSCMIDIOutputPort getOutputPort(void);
    void setOutputPort(VSCMIDIOutputPort port);     // throws if the output port could not be established
    
    void setController(VSCControllerPtr c);
    VSCControllerPtr getController(void);
    
    void sendMIDIStateIfRequired(void);
    
private:
    
    VSCMIDIOutputPort _outputPort;
    RtMidiOutPtr _midiOut;
    VSCControllerPtr _controller;
    
};

typedef boost::shared_ptr<VSCMIDIController> VSCMIDIControllerPtr;

#endif
