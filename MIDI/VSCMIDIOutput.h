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

#include "VSCSound.h"
#include "VSCMIDI.h"

#include "RtMidi.h"

#include <boost/shared_ptr.hpp>
#include <set>

namespace VSC {
    
    namespace MIDI {
        
        class Output {
            
        public:
            
            typedef boost::shared_ptr<Output> SPtr;
            
            Output(void);
            Output(OutputPort outputPort);
            
            OutputPort getOutputPort(void) const;
            
            // cannot send const (because RtMidi takes not const so would need to be copied)
            bool sendMessage(Message& m);
            
            bool sendNoteOn(unsigned int channel, unsigned int pitch, unsigned int velocity);
            bool sendNoteOff(unsigned int channel, unsigned int pitch, unsigned int velocity);
            bool sendControlChange(unsigned int channel, ControlNumber controlNumber, unsigned int value);
            bool sendPolyphonicAftertouch(unsigned int channel, unsigned int pitch, unsigned int pressure);
            bool sendChannelAftertouch(unsigned int channel, unsigned int pressure);
            
        protected:
            
            void setOutputPort(const OutputPort& port);     // throws if the output port could not be established
            
        private:
            
            typedef  boost::shared_ptr<RtMidiOut>    RtMidiOutPtr;
            
            OutputPort              mOutputPort;
            RtMidiOutPtr            mMIDIOut;
            
            MessageGenerator::SPtr  mMessageGenerator;
            
            void createRtMidiOut(void);
            
        };
        
        std::ostream& operator<<(std::ostream& output, const Output& p);

    }
    
}


#endif // _VSC_MIDI_OUTPUT_H_


