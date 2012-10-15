/*
 *  VSCBoost.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_MIDI_H_
#define _VSC_MIDI_H_

#include "VSCSound.h"
#include "VSCTaskQueue.h"

#include "RtMidi.h"

#include <boost/shared_ptr.hpp>

#include <list>
#include <string>
#include <vector>
#include <map>

namespace VSC {
    
    namespace MIDI {
        
        struct OutputPort {
            unsigned int number;
            std::string name;
            bool isVirtual;
            bool operator!=(const OutputPort& p) const;
            bool operator==(const OutputPort& p) const;
            bool operator<(const OutputPort& p) const;
        };
        
        std::ostream& operator<<(std::ostream& output, const OutputPort& p);
        
        struct InputPort {
            unsigned int number;
            std::string name;
            bool isVirtual;
            bool operator!=(const InputPort& p) const;
            bool operator==(const InputPort& p) const;
            bool operator<(const InputPort& p) const;
        };
        
        std::ostream& operator<<(std::ostream& output, const InputPort& p);
        
        extern const OutputPort     OutputPortVoid;
        extern const InputPort      InputPortVoid;
        
        typedef std::vector<OutputPort>    OutputPorts;
        typedef std::vector<InputPort>     InputPorts;
        
        /*
         *  A detailed table of midi messages can be found here http://www.midi.org/techspecs/midimessages.php
         *
         *  A bit of background information from wiki of MIDI messages http://en.wikipedia.org/wiki/MIDI_1.0
         *
         */
        
        enum ControlNumber {
            ControlInvalid                      = 999,
            ControlBankSelect                   = 0,
            ControlModulationWheel              = 1,
            ControlBreath                       = 2,
            ControlFootController               = 4,
            ControlChannelVolume                = 7,
            ControlBalance                      = 8,
            ControlUndefined1                   = 9,
            ControlPan                          = 10,
            ControlExpressionController         = 11,
            ControlEffectControl1               = 12,
            ControlEffectControl2               = 13,
            ControlUndefined2                   = 14,
            ControlUndefined3                   = 15,
            ControlGeneralPurposeController1    = 16,
            ControlGeneralPurposeController2    = 17,
            ControlGeneralPurposeController3    = 18,
            ControlGeneralPurposeController4    = 19,
            ControlUndefined4                   = 20,
            ControlUndefined5                   = 21,
            ControlUndefined6                   = 22,
            ControlUndefined7                   = 23,
            ControlUndefined8                   = 24,
            ControlUndefined9                   = 25,
            ControlUndefined10                  = 26,
            ControlUndefined11                  = 27,
            ControlUndefined12                  = 28,
            ControlUndefined13                  = 29,
            ControlUndefined14                  = 30,
            ControlUndefined15                  = 31
        };
        
        typedef std::vector<ControlNumber>  ControlNumbers;
        
        typedef std::vector<unsigned char>  Message;
        typedef std::pair<Message, Message> MessagePair;
        
        /*
         *  Message description utilities
         */
        
        std::string messageDescription(const Message& m);
        std::string controlNumberToString(ControlNumber num);
        
        class MessageGenerator {
            
        public:
            
            typedef boost::shared_ptr<MessageGenerator> SPtr;
            
            MessageGenerator();
            
            const ControlNumbers& getValidControlNumbers() {return mValidControlNumbers;}
            
            bool controlNumberIsValid(const ControlNumber& number);
            
            /*
             *  Note On/Off messages
             */
            
            Message messageForNote(unsigned int channel, unsigned int pitch, unsigned int velocity, bool on);
            
            /*
             *  Polyphonic aftertouch messages
             */
            
            Message messageForPolyphonicAftertouch(unsigned int channel, unsigned int pitch, unsigned int pressure);
            
            /*
             *  Channel Aftertouch messages, only channel and pressure are specified
             */
            
            Message messageForChannelAftertouch(unsigned int channel, unsigned int pressure);
            
            /*
             *  Low resolution control messages (only one message containing the MSB)
             */
            
            Message messageForControlChange(unsigned int channel, ControlNumber control, unsigned int value);
            
            /*
             *  Control messages can have MSB/LSB (sending two control message)
             *  http://www.logicprohelp.com/forum/viewtopic.php?f=5&t=81147
             *
             *  The second (LSB) message will be sent on the same channel, with controlNumber+32 as 
             *  described in the link above.
             *  
             *  value ranges from 0.00000 to 1.00000
             */
            
            MessagePair messagePairForControlChange(unsigned int channel, ControlNumber control, Float value);
            
            /*
             *  Pitch wheel messages have MSB/LSB within one MIDI message
             *
             *  value ranges from 0.00000 to 1.00000
             */
            
            Message messageForPitchWheel(unsigned int channel, Float value);
            
        private:
            
            /*
             *  Expects a float value within [0.0, 1.0]
             */
            
            bool floatValueToBytePair(Float value, unsigned char& MSB, unsigned char& LSB);
            
            ControlNumbers mValidControlNumbers;
            
        };
        
        class PortManager {
            
        public:
            
            typedef boost::shared_ptr<PortManager> SPtr;
            
            PortManager();
            ~PortManager(); // Not meant to be polymorphic, no virtual destructor

            void refreshInputPorts(void);
            void refreshOutputPorts(void);
            
            const OutputPorts& getOutputPorts(void) const;
            const InputPorts& getInputPorts(void) const;
            
            const std::string outputPortDescription(void) const;
            
        private:
            
            typedef  boost::shared_ptr<RtMidiIn>    RtMidiInPtr;
            typedef  boost::shared_ptr<RtMidiOut>   RtMidiOutPtr;
            
            RtMidiInPtr         mMIDIIn;
            RtMidiOutPtr        mMIDIOut;
            
            InputPorts          mInputPorts;
            OutputPorts         mOutputPorts;
            
        };
        
    }
    
}

#endif
