/*
 *  VSCSoundSine.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundSaw.h"


stk::BlitSaw* VSCSoundSaw::getStkBlitSaw(void) {
    return &blitSaw;
}

void VSCSoundSaw::updateSoundEngine(void) {
	
	// call superclass implementation
	VSCSoundGenerator::updateSoundEngine();
	
	/*
	 *	resize _computationFrames to have 1 channel (only need mono noise generation)
	 *	which will get spread to the (possibly) multi-channel VSCSoundGenerator
	 */
    if (_computationFrames.channels() != 1) {
        _computationFrames.resize(_computationFrames.frames(), 1);
    }
	
}