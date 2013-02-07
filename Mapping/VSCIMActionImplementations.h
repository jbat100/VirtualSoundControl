
#ifndef _VSC_IM_ACTION_IMPLEMENTATIONS_H_
#define _VSC_IM_ACTION_IMPLEMENTATIONS_H_

#include "VSCOB.h"
#include "VSCIMAction.h"
#include "VSCMIDI.h"
#include "VSCMIDIOwners.h"

#include <boost/shared_ptr.hpp>

namespace VSC {
    
    namespace IM {
        
        /*
         *  Concrete implementations
         */
        
        class Action::ImplementationMIDINoteOn : public Action::Implementation,
        public MIDI::OutputOwner, public MIDI::ChannelOwner
        {
            virtual const Tasks generateTasksWithTargetValueMap(Event::TargetValueMap& valueMap);
            virtual void setupMappings(Action::SPtr action);
        };
        
        class Action::ImplementationMIDINoteOff : public Action::Implementation,
        public MIDI::OutputOwner, public MIDI::ChannelOwner
        {
            virtual const Tasks generateTasksWithTargetValueMap(Event::TargetValueMap& valueMap);
            virtual void setupMappings(Action::SPtr action);
        };
        
        class Action::ImplementationMIDINoteOnAndOff : public Action::Implementation,
        public MIDI::OutputOwner, public MIDI::ChannelOwner
        {
            virtual const Tasks generateTasksWithTargetValueMap(Event::TargetValueMap& valueMap);
            virtual void setupMappings(Action::SPtr action);
        };
        
        class Action::ImplementationMIDIControlChange : public Action::Implementation,
        public MIDI::OutputOwner, public MIDI::ChannelOwner, public MIDI::ControlNumberOwner
        {
            virtual const Tasks generateTasksWithTargetValueMap(Event::TargetValueMap& valueMap);
            virtual void setupMappings(Action::SPtr action);
        };

    }
}

#endif // _VSC_IM_ACTION_IMPLEMENTATIONS_H_