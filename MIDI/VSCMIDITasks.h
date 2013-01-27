
#ifndef _VSC_MIDI_TASK_H_
#define _VSC_MIDI_TASK_H_

#include "VSC.h"
#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"
#include "VSCTask.h"

#include <boost/shared_ptr.hpp>

namespace VSC {
    
    namespace MIDI {
        
        /*
         *  Singleton MIDI task queue, all midi tasks should be queue through here
         *  to avoid concurrent MIDI operations.
         */
        
        TaskQueue::SPtr SingletonMIDITaskQueue();
        
        /*
         *  Base MIDI Task
         */
        
        class MIDITask : public Task
        {
            
        public:
            
            MIDITask(Task::Payload::SPtr payload) : Task(payload) {}
            
            Output::SPtr getMIDIOutput(void) {return mMIDIOutput;}
        
        private:
            
            Output::SPtr mMIDIOutput;
            
        };
        
        /*
         *  MIDI Send Message Task
         */
        
        class MIDISendMessageTask : public MIDITask
        {
            
        public:
            
            class Payload : public Task::Payload {
            public:
                Payload();
                typedef boost::shared_ptr<Payload> SPtr;
                Output::SPtr midiOutput;
                MessageDescription::SPtr messageDescription;
                TimeDuration timeOffset;
            };
            
            MIDISendMessageTask(Task::Payload::SPtr payload);
            
        protected:
            
            virtual bool stepExecution(void);
            
        private:
            
            static const bool mTraceExecution = true;
            
        };
        
    }
    
}

#endif
