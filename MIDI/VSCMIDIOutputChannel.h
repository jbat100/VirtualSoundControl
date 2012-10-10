/*
 *  VSCBoost.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_MIDI_OUTPUT_CHANNEL_H_
#define _VSC_MIDI_OUTPUT_CHANNEL_H_

#include "VSC.h"
#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"

#include <boost/shared_ptr.hpp>
#include <set>
#include <iostream>

namespace VSC {
    
    namespace MIDI {
        
        class OutputChannel {
            
        public:
            
            typedef boost::shared_ptr<OutputChannel> SPtr;
            
            OutputChannel(void);
            OutputChannel(Output::SPtr output, unsigned int channel = 0);
            
            /*
             *  Getters/Setters
             */
            
            MIDI::Output::SPtr getMIDIOutput(void) const {return mMIDIOutput;}
            void setMIDIOutput(MIDI::Output::SPtr output) {mMIDIOutput = output;}
            
            unsigned int getMIDIChannel(void) const {return mMIDIChannel;}
            void setMIDIChannel(unsigned int channel) {mMIDIChannel = channel;}
            
            /*
             *  MIDI message send API
             */
            
            bool sendNoteOn(unsigned int pitch, unsigned int velocity);
            bool sendNoteOff(unsigned int pitch, unsigned int velocity);
            bool sendControlChange(ControlNumber controlNumber, unsigned int value);
            bool sendPolyphonicAftertouch(unsigned int pitch, unsigned int pressure);
            bool sendChannelAftertouch(unsigned int pressure);
            
        private:
            
            Output::SPtr    mMIDIOutput;
            unsigned int    mMIDIChannel;
            
            MessageGenerator::SPtr  mMessageGenerator;
        };
        
        std::ostream& operator<<(std::ostream& output, const OutputChannel& p);

    }
    
}


#endif // _VSC_MIDI_OUTPUT_CHANNEL_H_


