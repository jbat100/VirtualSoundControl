
#ifndef _VSC_MIDI_INPUT_MANAGER_H_
#define _VSC_MIDI_INPUT_MANAGER_H_

#include "VSC.h"
#include "VSCBroadcaster.h"
#include "VSCMIDI.h"
#include "VSCMIDIInput.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <map>

namespace VSC {
    
    namespace MIDI {
                
        class InputManager : public VSC::Broadcaster, public boost::enable_shared_from_this<InputManager> {
            
        public:
            
            typedef boost::shared_ptr<InputManager> SPtr;
            
            friend class Input;
            
            class Listener : public VSC::Listener
            {
                public:
                
                typedef boost::shared_ptr<InputManager::Listener>  SPtr;
                typedef boost::weak_ptr<InputManager::Listener>    WPtr;
                
                virtual void midiInputManagerRefreshedInputs(InputManager::SPtr manager) {}
                
                virtual void midiInputReceivedMessage(InputManager::SPtr manager, Input::SPtr input, MessageDescription::SPtr description) {}
                
            };
            
            InputManager();
            virtual ~InputManager();
            
            static InputManager::SPtr singletonManager(void);
            
            void refreshInputs(void);
            
            Input::SPtr getFirstInput(void);
            Input::SPtr getFirstOpenedInput(void);
            Input::SPtr getInputWithDescription(const std::string& description);
            const Inputs& getInputs(void);
            
            const MessageQueue& getMessageQueue();
            
        protected:
            
            PortManager::SPtr getPortManager(void) {return mPortManager;}
            Input::SPtr getInputForPort(const InputPort& port);
            
            virtual bool checkListener(VSC::Listener::SPtr listener);
            
            void midiInputManagerRefreshedInputs(InputManager::SPtr manager) {}
            
            /*
             *  Called by friend class Input, so that the manager can propagate to all Listeners
             */
            
            void midiInputReceivedMessage(Input::SPtr input, MessageDescription::SPtr description) {}
            
        private:
            
            typedef boost::recursive_mutex Mutex;
            
            Inputs                  mInputs;
            PortManager::SPtr       mPortManager;
            Mutex                   mMutex;
            
            MessageQueue            mMessageQueue;
            
            static const bool       mTrace = true;
            
        };

    }    
}

#endif // _VSC_MIDI_OUTPUT_H_
