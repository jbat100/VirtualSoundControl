/*
 *  VSCBoost.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_MIDI_OUTPUT_H_
#define _VSC_MIDI_OUTPUT_H_

#include <set>
#include <boost/shared_ptr.hpp>

#include "VSCSound.h"
#include "VSCMIDI.h"

#include "RtMidi.h"

/*
 *  A detailed table of midi messages can be found here http://www.midi.org/techspecs/midimessages.php
 *
 *  A bit of background information from wiki of MIDI messages http://en.wikipedia.org/wiki/MIDI_1.0
 * 
 *  Using a lot of const referencer passing // http://www.parashift.com/c++-faq-lite/const-correctness.html
 *
 */


class VSCMIDIOutput {
    
public:
    
    VSCMIDIOutput(void);
    VSCMIDIOutput(VSCMIDIOutputPort outputPort);
    
    VSCMIDIOutputPort getOutputPort(void) const;
    void setOutputPort(VSCMIDIOutputPort const& port);     // throws if the output port could not be established
    
    bool sendMessage(VSCMIDI::Message const& m);
    
private:
    
    VSCMIDIOutputPort _outputPort;
    RtMidiOutPtr _midiOut;
    
};

typedef boost::shared_ptr<VSCMIDIOutput> VSCMIDIOutputPtr;

#endif // _VSC_MIDI_OUTPUT_H_


