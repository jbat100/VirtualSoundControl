/*
 *  VSCSoundSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_NOISE_
#define _VSC_SYNTH_SOURCE_NOISE_

#include "VSCSound.h"
#include "VSCSoundGenerator.h"
#include <boost/shared_ptr.hpp>

#define VSCSoundNoisePtr      boost::shared_ptr<VSCSoundNoise>

#ifdef VSCS_USE_STK

#include "Stk.h"
#include "Noise.h"

class VSCSoundNoise : public VSCSoundGenerator {
	
public:
    
    stk::Noise& getStkNoise(void);
	
private:

	stk::Noise noise;
    
};

inline VSCSFloat VSCSoundNoise::tick(void) {
	return noise.tick(_computationFrames);
}

#endif

#endif