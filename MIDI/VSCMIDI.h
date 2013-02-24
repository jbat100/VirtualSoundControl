
#ifndef _VSC_MIDI_H_
#define _VSC_MIDI_H_

#include "VSC.h"
#include "VSCSound.h"
#include "VSCTaskQueue.h"

#include "RtMidi.h"

#include <boost/shared_ptr.hpp>

#include <list>
#include <string>
#include <vector>
#include <deque>
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
         *  A bit of background information from wiki of MIDI messages http://en.wikipedia.org/wiki/MIDI_1.0
         */
        
        enum ControlNumber {
            ControlInvalid                      = 999,
            ControlNone                         = 998,
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
        
        enum MessageType
        {
            MessageTypeNone = 0,
            MessageTypeNoteOn,
            MessageTypeNoteOff,
            MessageTypePolyphonicAftertouch,
            MessageTypeChannelAftertouch,
            MessageTypeProgramChange,
            MessageTypeControlChange,
            MessageTypePitchWheel,
            MessageTypeInvalid
        };
        
        enum MessageParameterKey
        {
            MessageParameterKeyNone = 0,
            MessageParameterKeyChannel,
            MessageParameterKeyPitch,
            MessageParameterKeyVelocity,
            MessageParameterKeyPressure,
            MessageParameterKeyControlNumber,
            MessageParameterKeyValue,
            MessageParameterKeyValueMSB,
            MessageParameterKeyValueLSB,
            MessageParameterKeyProgram
        };
        
        typedef std::map<MessageParameterKey, unsigned char> MessageParameterMap;
        
        struct MessageDescription
        {
            MessageDescription(void) : type(MessageTypeNone) {}
            MessageDescription(MessageType t) : type(t) {}
            MessageDescription(MessageType t, MessageParameterMap m) : type(t), parameterMap(m) {}
            MessageDescription(const MessageDescription& description);
            typedef boost::shared_ptr<MessageDescription> SPtr;
            MessageType type;
            MessageParameterMap parameterMap;
        };
        
        typedef std::deque<MessageDescription::SPtr>    MessageDescriptionQueue;
        typedef std::vector<MessageDescription::SPtr>   MessageDescriptions;
        
        /*
         *  Message description utilities
         */
        
        std::string messageToString(const Message& m);
        std::string ControlNumberToString(ControlNumber num);
        ControlNumber StringToControlNumber(const std::string& desc);
        
        /*
         *  Expects a float value within [0.0, 1.0]
         */
        
        bool floatValueToBytePair(Float value, unsigned char& MSB, unsigned char& LSB);
        
        /*
         *  Convert between raw midi messages and human understandable description.
         */
        
        Message messageFromDescription(MessageDescription::SPtr description);
        
        MessageDescription::SPtr descriptionFromMessage(const Message& message);

        
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
