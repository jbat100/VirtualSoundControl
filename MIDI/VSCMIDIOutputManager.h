/*
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 */

#ifndef _VSC_MIDI_OUTPUT_MANAGER_H_
#define _VSC_MIDI_OUTPUT_MANAGER_H_

#include "VSC.h"
#include "VSCBroadcaster.h"
#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <map>

namespace VSC {
    
    namespace MIDI {
                
        class OutputManager : public VSC::Broadcaster, public boost::enable_shared_from_this<OutputManager> {
            
        public:
            
            typedef boost::shared_ptr<OutputManager> SPtr;
            
            class Listener : public VSC::Listener
            {
                public:
                
                typedef boost::shared_ptr<OutputManager::Listener>  SPtr;
                typedef boost::weak_ptr<OutputManager::Listener>    WPtr;
                
                void midiOutputManagerRefreshedOutputs(OutputManager::SPtr manager);
                
            };
            
            OutputManager();
            virtual ~OutputManager();
            
            static OutputManager::SPtr singletonManager(void);
            
            void refreshOutputs(void);
            
            Output::SPtr getFirstOutput(void);
            Output::SPtr getFirstOpenedOutput(void);
            Output::SPtr getOutputWithDescription(const std::string& description);
            const Outputs& getOutputs(void);
            
        protected:
            
            PortManager::SPtr getPortManager(void) {return mPortManager;}
            Output::SPtr getOutputForPort(const OutputPort& port);
            
            virtual bool checkListener(VSC::Listener::SPtr listener);
            
        private:
            
            typedef boost::recursive_mutex Mutex;
            
            Outputs                 mOutputs;
            PortManager::SPtr       mPortManager;
            Mutex                   mMutex;
            
            static const bool       mTrace = true;
            
        };

    }
    
}


#endif // _VSC_MIDI_OUTPUT_H_


