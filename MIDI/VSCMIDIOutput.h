#ifndef _VSC_MIDI_OUTPUT_H_
#define _VSC_MIDI_OUTPUT_H_

#include "VSC.h"
#include "VSCMIDI.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread.hpp>

#include <set>
#include <vector>

namespace VSC {
    
    namespace MIDI {
        
        class OutputManager;
                
        class Output {
            
        public:
            
            friend class OutputManager;
            
            typedef boost::shared_ptr<Output>   SPtr;
            typedef boost::weak_ptr<Output>     WPtr;
            
            enum State {
                StateNone = 0,
                StateClosed,
                StateOpened,
                StateUnknown
            };
            
            virtual ~Output();
            
            const OutputPort& getOutputPort(void) const;
            
            std::string getDescription(void);
            
            // cannot send const (because RtMidi takes not const so would need to be copied)
            bool sendMessage(Message& m);
            
            bool sendNoteOn(unsigned int channel, unsigned int pitch, unsigned int velocity);
            bool sendNoteOff(unsigned int channel, unsigned int pitch, unsigned int velocity);
            bool sendControlChange(unsigned int channel, ControlNumber controlNumber, unsigned int value);
            bool sendPolyphonicAftertouch(unsigned int channel, unsigned int pitch, unsigned int pressure);
            bool sendChannelAftertouch(unsigned int channel, unsigned int pressure);
            
            void open();
            void close();
            
            State getState(void) {return mState;}
            
            const ControlNumbers& getValidControlNumbers(void);
            bool controlNumberIsValid(const ControlNumber& number);
            
        protected:
            
            /*
             *  Contructors are made protected so that only friend class OutputManager can 
             *  be used to access them. One output object per port with internal mutex protection.
             */
            
            Output(const OutputPort& outputPort);
            
            void setOutputPort(const OutputPort& port);     // throws if the output port could not be established
            
            void setState(State state) {mState = state;}
            
        private:
            
            typedef  boost::shared_ptr<RtMidiOut>    RtMidiOutPtr;
            
            OutputPort              mOutputPort;
            RtMidiOutPtr            mMIDIOut;
            
            State                   mState;
            
            MessageGenerator::SPtr  mMessageGenerator;
            
            void createRtMidiOut(void);
            
            boost::mutex  mMutex;
            
        };
        
        std::ostream& operator<<(std::ostream& output, const Output& p);
        
        typedef std::vector<Output::SPtr> Outputs;

    }
    
}


#endif // _VSC_MIDI_OUTPUT_H_


