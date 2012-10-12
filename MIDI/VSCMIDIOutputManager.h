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
#include <map>

namespace VSC {
    
    namespace MIDI {
                
        class OutputManager {
            
        public:
            
            OutputManager();
            ~OutputManager();
            
            typedef boost::shared_ptr<OutputManager> SPtr;
            
            static OutputManager::SPtr singletonManager(void);
            
            PortManager::SPtr getPortManager(void) {return mPortManager;}
            
            Output::SPtr getOutputForPort(const OutputPort& port);
            Output::SPtr getFirstOutput(void);
            
        private:
            
            typedef std::map<OutputPort, Output::SPtr> OutputMap;
            
            OutputMap           mOutputMap;
            
            PortManager::SPtr   mPortManager;
            
        };

    }
    
}


#endif // _VSC_MIDI_OUTPUT_H_


