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
    

    stk::BlitSaw* getStkBlitSaw(void);
	
	void updateSoundEngine(void);
	
protected:
    
	stk::BlitSaw blitSaw;
    
    void processComputationFrames(void);
	
};

inline void VSCSynthSourceSaw::processComputationFrames(void) {
	blitSaw.tick(_computationFrames);
}

#endif

#endif