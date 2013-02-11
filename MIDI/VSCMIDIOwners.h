
#ifndef _VSC_MIDI_OWNERS_H_
#define _VSC_MIDI_OWNERS_H_

#include "VSC.h"
#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"

#include <boost/shared_ptr.hpp>


namespace VSC
{
    
    namespace MIDI
    {
        /*
         *  Very simple utility classes to avoid having to re-write loads of accessors...
         */
        
        class OutputOwner
        {
        public:
            typedef boost::shared_ptr<OutputOwner> SPtr;
            OutputOwner();
            Output::SPtr getMIDIOutput(void) {return mMIDIOutput;}
            void setMIDIOutput(Output::SPtr output) {mMIDIOutput = output;}
        private:
            Output::SPtr  mMIDIOutput;
        };
        
        class ChannelOwner
        {
        public:
            typedef boost::shared_ptr<ChannelOwner> SPtr;
            ChannelOwner() : mChannel(0) {}
            unsigned int getChannel(void) {return mChannel;}
            void setChannel(unsigned int chan) {mChannel = chan;}
        private:
            unsigned int mChannel;
        };
        
        class ControlNumberOwner
        {
        public:
            typedef boost::shared_ptr<ControlNumberOwner> SPtr;
            ControlNumberOwner() : mControlNumber(ControlInvalid) {}
            ControlNumber getControlNumber(void) {return mControlNumber;}
            void setControlNumber(ControlNumber number) {mControlNumber = number;}
        private:
            ControlNumber mControlNumber;
        };
    }
    
}

#endif // _VSC_MIDI_OWNERS_H_
