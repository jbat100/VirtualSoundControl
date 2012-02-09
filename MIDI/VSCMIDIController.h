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

#include <VSCSound>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class VSCMIDIController {
    
public:
    
    enum State {
        StateNone,
        StateStopped,
        StateRunning,
        StatePaused,
        StateEnded
    };
    
    VSCSFloat getCurrentValue(void) const;
    State getState(void) const;
    
private:
    
    State _state;
    VSCSFloat _currentTime;
    
};

typedef boost::shared_ptr<VSCMIDIController> VSCMIDIControllerPtr;

#endif
