/*
 *  VSCSynthSourceSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_FILE_
#define _VSC_SYNTH_SOURCE_FILE_

#include "VSCSynthSourceGenerator.h"
#include "VSCSound.h"

//#ifdef VSCS_USE_STK

#include "Stk.h"
#include "FileWvIn.h"

class VSCSynthSourceFile : public VSCSynthSourceGenerator {
	
public:
	
    
protected:
    
    stk::FileWvIn fileWvIn;
    
    void processComputationFrames(void);
	
};

/*

inline stk::StkFloat VSCSynthSourceFile::tick(unsigned int channel)
{
	if (_isOn) {
		stk::StkFloat normalizedValue = stk::FileWvIn::tick(channel);
		lastFrame_[0] = normalizedValue*_linearGain;
	}
	else {
		lastFrame_[0] = 0.0;
	}
	
	return lastFrame_[0];
}

inline stk::StkFrames& VSCSynthSourceFile::tick(stk::StkFrames& frames)
{
	stk::StkFloat *samples = &frames[0];
	unsigned int limit = frames.channels() * frames.frames();
	
	if (_isOn) {
		stk::FileWvIn::tick(frames);
		for (unsigned int i=0; i<limit; i++, samples ++) {
			*samples = (*samples) * _linearGain;
		}
	}
	else {
		for (unsigned int i=0; i<limit; i++, samples ++) {
			*samples = 0.0;
		}
	}
	
	lastFrame_[0] = *samples;
	return frames;
}
 
 */

//#endif // #ifdef VSCS_USE_STK

#endif // #ifndef _VSC_SYNTH_SOURCE_FILE_