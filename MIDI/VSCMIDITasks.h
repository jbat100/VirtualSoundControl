/*
 *  VSCBoost.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

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
            
            MIDITask(const Time& executionStartTime, Task::Payload::SPtr payload) : Task(executionStartTime, payload) {}
            
            Output::SPtr getMIDIOutput(void) {return mMIDIOutput;}
        
        private:
            
            Output::SPtr mMIDIOutput;
            
        };
        
        /*
         *  MIDI Note On Task
         */
        
        class MIDINoteOnTask : public MIDITask
        {
            
        public:
            
            class Payload : public Task::Payload {
            public:
                typedef boost::shared_ptr<Payload> SPtr;
                Output::SPtr midiOutput;
                unsigned int channel;
                unsigned int pitch;
                unsigned int velocity;
            };
            
            MIDINoteOnTask(const Time& executionStartTime, Task::Payload::SPtr payload);
            
        protected:
            
            virtual bool stepExecution(void);
            
        };
        
        /*
         *  MIDI Note Off Task
         */
        
        class MIDINoteOffTask : public MIDITask
        {
            
        public:
            
            class Payload : public Task::Payload {
            public:
                typedef boost::shared_ptr<Payload> SPtr;
                Output::SPtr midiOutput;
                unsigned int channel;
                unsigned int pitch;
                unsigned int velocity;
            };
            
            MIDINoteOffTask(const Time& executionStartTime, Task::Payload::SPtr payload);
            
        protected:
            
            virtual bool stepExecution(void);
            
        };
        
        /*
         *  MIDI Note On and Off Task
         */
        
        class MIDINoteOnAndOffTask : public MIDITask
        {
            
        public:
            
            class Payload : public Task::Payload {
            public:
                typedef boost::shared_ptr<Payload> SPtr;
                Output::SPtr midiOutput;
                unsigned int channel;
                unsigned int pitch;
                unsigned int onVelocity;
                unsigned int offVelocity;
                TimeDuration duration;
            };
            
            MIDINoteOnAndOffTask(const Time& executionStartTime, Task::Payload::SPtr payload);
            
        protected:
            
            virtual bool stepExecution(void);
            
        private:
            
            bool mSentNoteOn;
            
        };
        
        /*
         *  MIDI Control Change Task
         */
        
        class MIDIControlChangeTask : public MIDITask
        {
            
        public:
            
            class Payload : public Task::Payload {
            public:
                typedef boost::shared_ptr<Payload> SPtr;
                Output::SPtr midiOutput;
                unsigned int channel;
                ControlNumber controlNumber;
                unsigned int value;
            };
            
            MIDIControlChangeTask(const Time& executionStartTime, Task::Payload::SPtr payload);
            
        protected:
            
            virtual bool stepExecution(void);
            
        };
        
    }
    
}

#endif
