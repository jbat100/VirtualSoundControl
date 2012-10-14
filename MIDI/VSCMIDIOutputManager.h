/*
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 */

#ifndef _VSC_MIDI_OUTPUT_MANAGER_H_
#define _VSC_MIDI_OUTPUT_MANAGER_H_

#include "VSC.h"
#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <map>

namespace VSC {
    
    namespace MIDI {
                
        class OutputManager {
            
        public:
            
            OutputManager();
            ~OutputManager();
            
            typedef boost::shared_ptr<OutputManager> SPtr;
            
            static OutputManager::SPtr singletonManager(void);
            
            void refreshOutputs(void);
            
            Output::SPtr getFirstOutput(void);
            Output::SPtr getOutputWithDescription(const std::string& description);
            const Outputs& getOutputs(void);
            
        protected:
            
            PortManager::SPtr getPortManager(void) {return mPortManager;}
            Output::SPtr getOutputForPort(const OutputPort& port);
            
        private:
            
            typedef boost::recursive_mutex Mutex;
            
            Outputs                 mOutputs;
            PortManager::SPtr       mPortManager;
            Mutex                   mMutex;
            
        };

    }
    
}


#endif // _VSC_MIDI_OUTPUT_H_


