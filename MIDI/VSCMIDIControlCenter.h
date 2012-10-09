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

#include "VSCSound.h"
#include "VSCMIDI.h"
#include "VSCMIDIController.h"

#include "RtMidi.h"

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

namespace VSC {
    
    namespace MIDI {
        
        
        /**
         *  ControlCenter should be used for:
         *
         *  - Getting available MIDI input and output ports.
         *  - Periodically sending controller values on a background thread.
         */
        
        class ControlCenter {
            
        public:
            
            typedef boost::shared_ptr<ControlCenter> SPtr;
            
            enum State {
                StateNone = 0,
                StateStopped,
                StatePeriodicBroadcasting
            };
            
            typedef std::vector<Controller::SPtr> ControllerList;
            
            ControlCenter(void);
            ~ControlCenter(void);
            
            void addController(Controller controller);
            void removeController(Controller controller);
            const Controllers& getControllers(void) const;
            
            State getState(void);
            
            void broadcastNow(void);
            void startPeriodicBroadcasting(void);
            void stopPeriodicBroadcasting(void);
            
            VSC::Sound::TimeInterval getBroadcastInterval(void) const;
            void setBroadcastInterval(const VSC::Sound::TimeInterval interval);
            
        protected:
            
            void setState(State state);
            
        private:
            
            Controllers                 mControllers;
            
            State                       mState;
            VSC::Sound::TimeInterval    mBroadcastInterval;
            
            // static boost::mutex         mSingletonMutex;
            
            
        };
        

        
    }
    
}

#endif