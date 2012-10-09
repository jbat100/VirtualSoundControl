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

#include "VSCSound.h"
#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"
#include "VSCController.h"

#include "RtMidi.h"

#include <set>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace VSC {
    
    namespace MIDI {
    
        
        /*
         *  A detailed table of midi messages can be found here http://www.midi.org/techspecs/midimessages.php
         *
         *  A bit of background information from wiki of MIDI messages http://en.wikipedia.org/wiki/MIDI_1.0
         *
         */
        
        
        class ControllerBinding {
            
        public:
            
            typedef boost::shared_ptr<ControllerBinding> SPtr;
            
            enum Mode {
                ModeNone = 0,
                ModeControlLowResolution,
                ModeControlHighResolution,
                ModeChannelAftertouch,
                ModePolyphonicAftertouch,
                ModePitchWheel
            };
            
            ControllerBinding(void);
            
            bool sendCurrentValue(void);
            
            void setController(VSC::Controller::SPtr c);
            VSC::Controller::SPtr getController(void);
            
            void setOutput(Output::SPtr o);
            Output::SPtr getMIDIOutput(void);
            
            void setMIDIChannel(const unsigned int chan);
            unsigned int getMIDIChannel(void) const;
            
            /*
             *  Mode specific
             */
            
            void setControlNumber(const ControlNumber num);
            ControlNumber getControlNumber(void) const;
            
            void setMIDIPitch(unsigned int pitch);
            unsigned int getMIDIPitch(void);
            
        private:
            
            Output::SPtr            mMIDIOutput;
            VSC::Controller::SPtr   mController;
            
            unsigned int            mMIDIChannel;
            ControlNumber           mControlNumber;
            
            Mode                    mMode;
            unsigned int            mMIDIPitch;
        };
        
        typedef std::vector<ControllerBinding::SPtr> ControllerBindings;
        
    }
    
}

#endif


