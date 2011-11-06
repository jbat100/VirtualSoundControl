/*
 *  VSCSynthSourceSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_NOISE_
#define _VSC_SYNTH_SOURCE_NOISE_

#include "VSCSound.h"
#include "VSCSynthSourceGenerator.h"

#ifdef VSCS_USE_STK

#include "Stk.h"
#include "Noise.h"

class VSCSynthSourceNoise : public VSCSynthSourceGenerator {
	
public:
    
    stk::Noise* getStkNoise(void);
	
	void updateSoundEngine(void);
	
protected:

	stk::Noise noise;
    void processComputationFrames(void);
    
};

inline void VSCSynthSourceNoise::processComputationFrames(void) {
	noise.tick(_computationFrames);
}

#endif

#endif