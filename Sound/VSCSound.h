/*
 *  VSCSound.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 22/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 *
 *	General definitions for everything that has to do with sound stuff
 *
 */


#ifndef _VSC_SOUND_H_
#define _VSC_SOUND_H_

#include <boost/shared_ptr.hpp>

//#include <climits>
#include <vector>
#include <string>

namespace VSC {
    
    namespace Sound {
        
        /*-------------------------------------------------------------------------------------------
         *  Just to avoid silly values for numbers of channels, accidental or not...
         */
        const unsigned int kMaxChannels = 16;
        
        /*
         *  Define VSCS_ALL_CHANNELS to specify audio processing tick is to occur for all channels
         *
         *  This is used in the stk::generator call
         *  stk::StkFrames& tick(stk::StkFrames& frames, unsigned int channel = 0);
         */
        const unsigned int kAllChannels = UINT_MAX;
        const unsigned int kVoidChannel = UINT_MAX - 1;
        
        /*-------------------------------------------------------------------------------------------*/
        
        typedef double Float;
        
        typedef std::vector<Float> Floats;
        
        /*-------------------------------------------------------------------------------------------*/
        
        typedef Float Time;             // absolute time (seconds)
        typedef Float TimeInterval;     // absolute time interval (seconds)
        typedef Float ControlValue;     // VSC sound control value (for MIDI, OSC ...)
        
        
        /*-------------------------------------------------------------------------------------------
         *
         *	VSCSound class definition providing general purpose sound related
         *	functionality.
         */
        
        class Pitch {
            
        public:
            
            typedef boost::shared_ptr<Pitch> SPtr;
            
            Pitch();
            
            /*
             *	0.0 < midiNote < 127.0
             */
            Float frequencyForMidiNote(Float midiNote);
            Float frequencyForMidiNote(unsigned int midiNote);
            
            void setReferenceAFrequency(Float f);
            Float getReferenceAFrequency(void);
            
            Float logFrequencyToFrequency(Float logFreq);
            Float frequencyToLogFrequency(Float logFreq);
            
        private:
            
            void computeMidiNoteFrequencies(void);
            
            Floats mMIDINoteFrequencies;
            
            Float mReferenceAFrequency; // a is 440 hz...
            
        };
        
    }
    
}

#endif


