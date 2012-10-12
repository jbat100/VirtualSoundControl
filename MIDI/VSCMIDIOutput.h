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

#include "VSC.h"
#include "VSCMIDI.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <set>

namespace VSC {
    
    namespace MIDI {
        
        class OutputManager;
                
        class Output {
            
        public:
            
            friend class OutputManager;
            
            typedef boost::shared_ptr<Output> SPtr;
            
            virtual ~Output();
            
            const OutputPort& getOutputPort(void) const;
            
            // cannot send const (because RtMidi takes not const so would need to be copied)
            bool sendMessage(Message& m);
            
            bool sendNoteOn(unsigned int channel, unsigned int pitch, unsigned int velocity);
            bool sendNoteOff(unsigned int channel, unsigned int pitch, unsigned int velocity);
            bool sendControlChange(unsigned int channel, ControlNumber controlNumber, unsigned int value);
            bool sendPolyphonicAftertouch(unsigned int channel, unsigned int pitch, unsigned int pressure);
            bool sendChannelAftertouch(unsigned int channel, unsigned int pressure);
            
            void reinitialize(); // If operations fail try reinitialize
            
        protected:
            
            /*
             *  Contructors are made protected so that only friend class OutputManager can 
             *  be used to access them. One output object per port with internal mutex protection.
             */
            
            Output(void);
            Output(const OutputPort& outputPort);
            
            void setOutputPort(const OutputPort& port);     // throws if the output port could not be established
            
        private:
            
            typedef  boost::shared_ptr<RtMidiOut>    RtMidiOutPtr;
            
            OutputPort              mOutputPort;
            RtMidiOutPtr            mMIDIOut;
            
            MessageGenerator::SPtr  mMessageGenerator;
            
            void createRtMidiOut(void);
            
            boost::mutex            mMutex;
            
        };
        
        std::ostream& operator<<(std::ostream& output, const Output& p);

    }
    
}


#endif // _VSC_MIDI_OUTPUT_H_


