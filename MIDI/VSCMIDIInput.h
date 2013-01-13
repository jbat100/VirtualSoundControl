
#ifndef _VSC_MIDI_INPUT_H_
#define _VSC_MIDI_INPUT_H_

#include "VSC.h"
#include "VSCMIDI.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <set>
#include <vector>

namespace VSC {
    
    namespace MIDI {
        
        class InputManager;
                
        class Input : public boost::enable_shared_from_this<Input> {
            
        public:
            
            friend class InputManager;
            
            typedef boost::shared_ptr<Input>    SPtr;
            typedef boost::weak_ptr<Input>      WPtr;
            
            enum State {
                StateNone = 0,
                StateClosed,
                StateOpened,
                StateUnknown
            };
            
            virtual ~Input();
            
            const InputPort& getInputPort(void) const;
            
            std::string getDescription(void);
            
            void open();
            void close();
            
            State getState(void) {return mState;}
            
        protected:
            
            /*
             *  Contructors are made protected so that only friend class InputManager can 
             *  be used to access them. One input object per port with internal mutex protection.
             */
            
            Input(const InputPort& inputPort);
            
            void setInputPort(const InputPort& port);     // throws if the input port could not be established
            
            void setState(State state) {mState = state;}
            
        private:
            
            typedef  boost::shared_ptr<RtMidiIn>    RtMidiInPtr;
            
            InputPort               mInputPort;
            RtMidiInPtr             mMIDIIn;
            
            State                   mState;
            
            void createRtMidiIn(void);
            
            boost::mutex  mMutex;
            
        };
        
        std::ostream& operator<<(std::ostream& output, const Input& p);
        
        typedef std::vector<Input::SPtr> Inputs;

    }
    
}


#endif // _VSC_MIDI_INPUT_H_


