
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
        
        
        class MIDIAction : public Action {
            
        public:
            
            typedef boost::shared_ptr<MIDIAction> SPtr;
            
            MIDIAction();
            
            MIDI::Output::SPtr getMIDIOutput(void) {return mMIDIOutput;}
            void setMIDIOutput(MIDI::Output::SPtr output) {mMIDIOutput = output;}
            
            unsigned int getChannel(void) {return mChannel;}
            void setChannel(unsigned int chan) {mChannel = chan;}
            
        private:
            
            MIDI::Output::SPtr  mMIDIOutput;
            
            unsigned int mChannel;
            
        };
        
        
        class MIDIControlAction : public MIDIAction
        {
            
        public:
            
            typedef boost::shared_ptr<MIDIControlAction> SPtr;
            
            MIDIControlAction();
            
            MIDI::ControlNumber getControlNumber(void) {return mControlNumber;}
            void setControlNumber(MIDI::ControlNumber number) {mControlNumber = number;}
            
        private:
            
            MIDI::ControlNumber mControlNumber;
            
        };
        
        /*
         *  Generates a note on task
         */
        
        class MIDINoteOnAction : public MIDIAction
        {
            
        public:
            
            MIDINoteOnAction();
            
            virtual void createDefaultMappings();
            
            virtual const Tasks generateTasksWithValueMap(Action::ValueMap& valueMap);
        };
        
        /*
         *  Generates a note off task
         */
        
        class MIDINoteOffAction : public MIDIAction
        {
            
        public:
            
            MIDINoteOffAction();
            
            virtual void createDefaultMappings();
            
            virtual const Tasks generateTasksWithValueMap(Action::ValueMap& valueMap);
        };
        
        /*
         *  Generates a note on task followed by a note off task after a mapping derived duration
         */
        
        class MIDINoteOnAndOffAction : public MIDIAction
        {
            
        public:
            
            MIDINoteOnAndOffAction();
            
            virtual void createDefaultMappings();
            
            virtual const Tasks generateTasksWithValueMap(Action::ValueMap& valueMap);
            
        };
        
        class MIDIControlChangeAction : public MIDIControlAction
        {
            
        public:
            
            MIDIControlChangeAction();
            
            virtual void createDefaultMappings();
            
            virtual const Tasks generateTasksWithValueMap(Action::ValueMap& valueMap);
            
        };
        
        /*
         *  Utilies and shortcuts
         */
        
        bool actionIsMIDIAction(Action::SPtr action);
    
        bool actionIsMIDIActionControlAction(Action::SPtr action);

    }
    
}

#endif // _VSC_IM_MIDI_ACTION_H_
