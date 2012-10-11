/*
 *  VSCBoost.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_MIDI_H_
#define _VSC_MIDI_H_

#include "VSC.h"
#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"
#include "VSCTask.h"


namespace VSC {
    
    namespace MIDI {
        
        /*
         *  Base MIDI Task
         */
        
        class MIDITask : public Task
        {
            
        public:
            
            MIDITask(Output::SPtr output) : Task(), mMIDIOutput(output) {}
            
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
            
            MIDINoteOnTask(Output::SPtr output,
                           unsigned int channel,
                           unsigned int pitch,
                           unsigned int velocity);
            
        protected:
            
            virtual bool stepExecution(void);
            
        private:
            
            unsigned int mChannel;
            unsigned int mPitch;
            unsigned int mVelocity;
            
        };
        
        /*
         *  MIDI Note Off Task
         */
        
        class MIDINoteOffTask : public MIDITask
        {
            
        public:
            
            MIDINoteOffTask(Output::SPtr output,
                            unsigned int channel,
                            unsigned int pitch,
                            unsigned int velocity);
            
        protected:
            
            virtual bool stepExecution(void);
            
        private:
            
            unsigned int mChannel;
            unsigned int mPitch;
            unsigned int mVelocity;
            
        };
        
        /*
         *  MIDI Note On and Off Task
         */
        
        class MIDINoteOnAndOffTask : public MIDITask
        {
            
        public:
            
            MIDINoteOnAndOffTask(Output::SPtr output,
                                 unsigned int channel,
                                 unsigned int pitch,
                                 unsigned int onVelocity,
                                 unsigned int offVelocity,
                                 TimeDuration duration);
            
        protected:
            
            virtual bool stepExecution(void);
            
        private:
            
            unsigned int mChannel;
            unsigned int mPitch;
            unsigned int mOnVelocity;
            unsigned int mOffVelocity;
            
            TimeDuration mDuration;
            
            bool        mSentNoteOn;
            
        };
        
        /*
         *  MIDI Control Change Task
         */
        
        class MIDIControlChangeTask : public MIDITask
        {
            
        public:
            
            MIDIControlChangeTask(Output::SPtr output,
                                  unsigned int channel,
                                  ControlNumber controlNumber,
                                  unsigned int value);
            
        protected:
            
            virtual bool stepExecution(void);
            
        private:
            
            unsigned int    mChannel;
            unsigned int    mValue;
            
            ControlNumber   mControlNumber;
            
        };
        
    }
    
}

#endif
