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

#include "Stk.h"
#include "SineWave.h"

class VSCSynthSourceSine : public VSCSynthSourceGenerator {
	
public:
    
    stk::SineWave* getStkSineWave(void);
	
	void updateSoundEngine(void);
	
protected:
    
	stk::SineWave sineWave;
    
    void processComputationFrames(void);
	
};

inline void VSCSynthSourceSine::processComputationFrames(void) {
	sineWave.tick(_computationFrames);
}


#endif

#endif