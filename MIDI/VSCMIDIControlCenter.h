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
#include "VSCMIDIControllerBinding.h"

#include "RtMidi.h"

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

namespace VSC {
    
    namespace MIDI {
        
        
        /**
         *  ControlCenter should be used for:
         *
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
            
            ControlCenter(void);
            ~ControlCenter(void);
            
            void addControllerBinding(ControllerBinding::SPtr binding);
            void removeControllerBinding(ControllerBinding::SPtr binding);
            
            const ControllerBindings& getControllerBindings(void) const;
            
            State getState(void);
            
            void broadcastNow(void);
            
            void startPeriodicBroadcasting(void);
            void stopPeriodicBroadcasting(void);
            
            TimeDuration getBroadcastInterval(void) const;
            void setBroadcastInterval(const TimeDuration& interval);
            
        protected:
            
            void setState(State state);
            
        private:
            
            ControllerBindings          mControllerBindings;
            
            State                       mState;
            TimeDuration                mBroadcastInterval;
            
            // static boost::mutex         mSingletonMutex;
            
            
        };
        

        
    }
    
}

#endif