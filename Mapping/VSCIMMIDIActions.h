
#ifndef _VSC_IM_MIDI_ACTION_H_
#define _VSC_IM_MIDI_ACTION_H_

#include "VSCOB.h"
#include "VSCIMCollisionMapping.h"
#include "VSCIMAction.h"
#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        /*
         *  Implementations info owners
         */
        
        class MIDIOutputOwner
        {
        public:
            typedef boost::shared_ptr<MIDIOutputOwner> SPtr;
            MIDIOutputOwner(): mMIDIOutput(MIDI::Output::SPtr()) {}
            MIDI::Output::SPtr getMIDIOutput(void) {return mMIDIOutput;}
            void setMIDIOutput(MIDI::Output::SPtr output) {mMIDIOutput = output;}
        private:
            MIDI::Output::SPtr  mMIDIOutput;
        };
        
        class MIDIChannelOwner
        {
        public:
            typedef boost::shared_ptr<MIDIChannelOwner> SPtr;
            MIDIChannelOwner() : mChannel(0) {}
            unsigned int getChannel(void) {return mChannel;}
            void setChannel(unsigned int chan) {mChannel = chan;}
        private:
            unsigned int mChannel;
        }
        
        class MIDIControlNumberOwner
        {
        public:
            MIDIControlNumberOwner() : mControlNumber(MIDI::ControlInvalid) {}
            MIDI::ControlNumber getControlNumber(void) {return mControlNumber;}
            void setControlNumber(MIDI::ControlNumber number) {mControlNumber = number;}
        private:
            MIDI::ControlNumber mControlNumber;
        };
        
        /*
         *  Concrete implementations
         */
        
        class Action::ImplementationMIDINoteOn : public Action::Implementation,
        public MIDIOutputOwner, public MIDIChannelOwner
        {
            virtual const Tasks generateTasksWithValueMap(Event::ValueMap& valueMap);
            virtual void createDefaultMappings();
        };
        
        class Action::ImplementationMIDINoteOff : public Action::Implementation,
        public MIDIOutputOwner, public MIDIChannelOwner
        {
            virtual const Tasks generateTasksWithValueMap(Event::ValueMap& valueMap);
            virtual void createDefaultMappings();
        };
        
        class Action::ImplementationMIDINoteOnAndOff : public Action::Implementation,
        public MIDIOutputOwner, public MIDIChannelOwner
        {
            virtual const Tasks generateTasksWithValueMap(Event::ValueMap& valueMap);
            virtual void createDefaultMappings();
        };
        
        class Action::ImplementationMIDIControlChange : public Action::Implementation,
        public MIDIOutputOwner, public MIDIChannelOwner, public MIDIControlNumberOwner
        {
            virtual const Tasks generateTasksWithValueMap(Event::ValueMap& valueMap);
            virtual void createDefaultMappings();
        };

    }
    
}

#endif // _VSC_IM_MIDI_ACTION_H_
