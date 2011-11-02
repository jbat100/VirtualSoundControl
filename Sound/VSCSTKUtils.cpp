/*
 *  VSCSTKUtils.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 01/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSound.h"
#include "VSCSTKUtils.h"
#include <cassert>

#include <Accelerate/Accelerate.h>

namespace stk {
    
	void averageFramesChannels(StkFrames& frames, StkFrames& averagedFrames) {
		
		/*
		 *  resize averagedFramed with the same number of frames as frames
		 */
		averagedFrames.resize(frames.frames(), 1);
		
		for (unsigned int frameIndex = 0; frameIndex < frames.frames(); frameIndex++) {
			StkFloat average = 0.0;
			for (unsigned int channelIndex = 0; channelIndex < frames.channels(); channelIndex++) {
				average += frames(frameIndex, channelIndex);
			}
			average /= (StkFloat)frames.channels();
			averagedFrames[frameIndex] = average;
		}
		
	}

	void setFramesChannel(StkFrames& targetFrames, StkFrames& monoFrames, unsigned int channel) {
		
		assert(targetFrames.frames() == monoFrames.frames());
		assert(monoFrames.channels() == 1);
		assert(targetFrames.channels() > channel);
		
		for (unsigned int frameIndex = 0; frameIndex < targetFrames.frames(); frameIndex++) {
			targetFrames(frameIndex, channel) = monoFrames[frameIndex];
		}
		
	}
	
	void scaleFramesChannel(StkFrames& targetFrames, unsigned int channel, StkFloat scale) {
		
		assert(targetFrames.channels() > channel);
		
		if (targetFrames.frames() < 1) 
			return;
		
#ifdef VSCS_USE_DOUBLE_PRECISION
		vDSP_vsmulD(&targetFrames[channel], targetFrames.channels(), &scale, 
				    &targetFrames[channel], targetFrames.channels(), targetFrames.frames());
#else
		vDSP_vsmul(&targetFrames[channel], targetFrames.channels(), &scale, 
				   &targetFrames[channel], targetFrames.channels(), targetFrames.frames());	
#endif
		
	}

}

