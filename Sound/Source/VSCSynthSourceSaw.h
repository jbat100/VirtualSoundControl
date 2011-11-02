/*
 *  VSCSynthSourceSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_SAW_
#define _VSC_SYNTH_SOURCE_SAW_

#include "VSCSound.h"
#include "VSCSynthSourceGenerator.h"

#ifdef VSCS_USE_STK

#include "BlitSaw.h"

class VSCSynthSourceSaw : public VSCSynthSourceGenerator {
	
public:
	
protected:
    
	stk::BlitSaw blitSaw;
    
    void processComputationFrames(void);
	
};

inline void VSCSynthSourceSaw::processComputationFrames(void) {
	
}

/*

inline stk::StkFloat VSCSynthSourceSaw::tick(void)
{
	if (_isOn) {
		stk::StkFloat normalizedValue = stk::BlitSaw::tick();
		lastFrame_[0] = normalizedValue*_linearGain;
	}
	else {
		//stk::StkFloat normalizedValue = 
		stk::BlitSaw::tick();
		lastFrame_[0] = 0.0;
	}
	
	return lastFrame_[0];
}

inline stk::StkFrames& VSCSynthSourceSaw::tick(stk::StkFrames& frames, unsigned int channel)
{
	stk::StkFloat *samples = &frames[channel];
	unsigned int hop = frames.channels();
	if (_isOn) {
		stk::BlitSaw::tick(frames, channel);
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