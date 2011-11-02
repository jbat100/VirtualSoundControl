/*
 *  VSCSynthSourceSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_FILE_LOOP_
#define _VSC_SYNTH_SOURCE_FILE_LOOP_

#include "VSCSynthSourceGenerator.h"
#include "VSCSound.h"

#ifdef VSCS_USE_STK

#include "FileLoop.h"

class VSCSynthSourceFileLoop : public VSCSynthSourceGenerator {
	
public:
	
    
protected:
    
    stk::FileLoop fileWvIn;
    
    void processComputationFrames(void);
	
};

/*

inline stk::StkFloat VSCSynthSourceFileLoop::tick(unsigned int channel)
{
	if (_isOn) {
		stk::StkFloat normalizedValue = stk::FileLoop::tick(channel);
		lastFrame_[0] = normalizedValue*_linearGain;
	}
	else {
		lastFrame_[0] = 0.0;
	}
	
	return lastFrame_[0];
}

inline stk::StkFrames& VSCSynthSourceFileLoop::tick(stk::StkFrames& frames)
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

#endif // #ifdef VSCS_USE_STK

#endif // #ifndef _VSC_SYNTH_SOURCE_FILE_LOOP_