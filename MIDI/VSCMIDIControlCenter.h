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
#include <boost/thread/mutex.hpp>


#include "RtMidi.h"

#include "VSCSound.h"
#include "VSCMIDI.h"
#include "VSCMIDIController.h"

class VSCMIDIControlCenter;

typedef boost::shared_ptr<VSCMIDIControlCenter> VSCMIDIControlCenterPtr;

/**
 *  VSCMIDIControlCenter should be used for:
 *
 *  - Getting available MIDI input and output ports.
 *  - Periodically sending controller values on a background thread.
 */

class VSCMIDIControlCenter {
    
public:
    
    enum State {
        StateNone = 0,
        StateStopped,
        StatePeriodicBroadcasting
    };
    
    typedef std::list<VSCMIDIControllerPtr> ControllerList;
    
    VSCMIDIControlCenter(void);
    ~VSCMIDIControlCenter(void);
    
    static VSCMIDIControlCenterPtr defaultCenter(void);
    
    void addController(VSCMIDIControllerPtr controller);
    void removeController(VSCMIDIControllerPtr controller);
    const ControllerList& getControllerList(void) const;
    
    State getState(void);
    
    void broadcastNow(void);
    void startPeriodicBroadcasting(void);
    void stopPeriodicBroadcasting(void);
    
    VSCSTimeInterval getBroadcastInterval(void) const;
    void setBroadcastInterval(const VSCSTimeInterval interval);
    
protected:
    
    void setState(State state);
    
private:
    
    ControllerList _controllerList;
    
    State _state;
    VSCSTimeInterval _broadcastInterval;
    
    static boost::mutex _singletonMutex;

    
};


#endif