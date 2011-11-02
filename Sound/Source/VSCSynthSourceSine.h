/*
 *  VSCSynthSourceSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_SINE_
#define _VSC_SYNTH_SOURCE_SINE_

#include "VSCSound.h"
#include "VSCSynthSourceGenerator.h"

#ifdef VSCS_USE_STK

#include "SineWave.h"

class VSCSynthSourceSine : public VSCSynthSourceGenerator {
	
public:
	
protected:
    
	stk::SineWave sineWave;
    
    void processComputationFrames(void);
	
};

inline void VSCSynthSourceSine::processComputationFrames(void) {
	
}

/*

inline stk::StkFloat VSCSynthSourceSine::tick(void)
{
	if (_isOn) {
		stk::StkFloat normalizedValue = stk::SineWave::tick();
		lastFrame_[0] = normalizedValue*_linearGain;
	}
	else {
		stk::StkFloat normalizedValue = stk::SineWave::tick();
		lastFrame_[0] = 0.0;
	}
	
	return lastFrame_[0];
}

inline stk::StkFrames& VSCSynthSourceSine::tick(stk::StkFrames& frames, unsigned int channel)
{
	stk::StkFloat *samples = &frames[channel];
	unsigned int hop = frames.channels();
	if (_isOn) {
		stk::SineWave::tick(frames, channel);
		for (unsigned int i=0; i<frames.frames(); i++, samples += hop) {
			*samples = (*samples) * _linearGain;
		}		
	}
	else {
		for (unsigned int i=0; i<frames.frames(); i++, samples += hop) {
			*samples = 0.0;
		}	
	}
	lastFrame_[0] = *samples;
	return frames;
}
 
 */

#endif

#endif