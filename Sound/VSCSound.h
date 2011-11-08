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

#include <climits>
#include <vector>

#define VSCS_USE_STK

/*
 *  Define whether the sound part of VSC (VSCS) should do processing in double or float (this should be in accordance with 
 *  the type of VSCSFloat). Useful for calling different Accelerate framework methods depending on precision for example.
 */
#define VSCS_USE_DOUBLE_PRECISION

/*
 *  Just to avoid silly values for numbers of channels, accidental or not...
 */
const unsigned int kVSCSMaxChannels = 16;

/*
 *  Define VSCS_ALL_CHANNELS to specify audio processing tick is to occur for all channels 
 *  
 *  This is used in the stk::generator call
 *  stk::StkFrames& tick(stk::StkFrames& frames, unsigned int channel = 0);
 */
const unsigned int kVSCSAllChannels = UINT_MAX;

#ifdef VSCS_USE_STK
	#include "Stk.h"
	//Define VSCSFloat (double or float or same as stk::StkFloat)
	typedef stk::StkFloat VSCSFloat;
#else 
	typedef double VSCSFloat;
#endif


class VSCSound {
	
public:
	
	static VSCSFloat linearToDBGain(VSCSFloat linearGain);
	static VSCSFloat dBToLinearGain(VSCSFloat dBGain);
	
	/*
	 *	0.0 < midiNote < 127.0
	 */
	static VSCSFloat frequencyForMidiNote(VSCSFloat midiNote);
	static VSCSFloat frequencyForMidiNote(unsigned int midiNote);
	
	static void setReferenceAFrequency(VSCSFloat f);
	static VSCSFloat getReferenceAFrequency(void);
	
private:
	
	static void computeMidiNoteFrequencies(void);
	
	static std::vector<VSCSFloat> midiNoteFrequencies;
	static VSCSFloat referenceAFrequency; // a is 440 hz...
	
	static bool midiNoteFrequenciesAreComputed;
	
};

#endif

