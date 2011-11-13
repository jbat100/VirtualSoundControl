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
#include <string>
#include <iostream>

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

	void setFramesChannelWithMonoFrames(StkFrames& targetFrames, StkFrames& monoFrames, unsigned int channel) {
		
		assert(targetFrames.frames() == monoFrames.frames());
		assert(monoFrames.channels() == 1);
		assert(targetFrames.channels() > channel);
		
		for (unsigned int frameIndex = 0; frameIndex < targetFrames.frames(); frameIndex++) {
			targetFrames(frameIndex, channel) = monoFrames[frameIndex];
		}
		
	}
    
    void setAllFramesChannelsWithMonoFrames(StkFrames& targetFrames, StkFrames& monoFrames) {
        for (unsigned int i = 0; i < targetFrames.channels(); i++) {
            setFramesChannelWithMonoFrames(targetFrames, monoFrames, i);
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
    
    void zeroFrames(StkFrames& targetFrames) {
        
        std::memset(&targetFrames[0], 0, sizeof(stk::StkFloat) * targetFrames.size());
        
    }
	
	void zeroFrame(StkFrames& targetFrames, unsigned int frameIndex) {
        
		assert(targetFrames.frames() > frameIndex);
		
		if (targetFrames.frames() <= frameIndex) {
			return;
		}
		
        for (unsigned int channelIndex = 0; channelIndex < targetFrames.channels(); channelIndex++) {
			targetFrames(frameIndex, channelIndex) = 0.0;
		}
        
    }
	
	void shiftFrames(StkFrames& frames, int shift) {
		
		if (std::abs(shift) > frames.frames()) {
			// if we shift by more than the frame length then might as well zero the whole thing
			zeroFrames(frames);
		}
		
		else if (shift > 1) {
			// if the shift is positive, then go backwards to not overwrite needed information
			for (unsigned int frameIndex = frames.frames()-shift-1; frameIndex >= 0; frameIndex--) {
				// if we are within the range
				if (frameIndex+shift < frames.frames()) {
					// transfer the frame data to the shifted location
					for (unsigned int channelIndex = 0; channelIndex , frames.channels(); channelIndex++) {
						frames(frameIndex+shift, channelIndex) = frames(frameIndex, channelIndex);
					}
				}
			}
			// zero the frames which do not have valid data
			for (unsigned int frameIndex = 0; frameIndex < shift; frameIndex++) {
				zeroFrame(frames, frameIndex);
			}
		}
		
		else if (shift < 1) {
			// if the shift is positive, then go backwards to not overwrite needed information
			for (unsigned int frameIndex = 0; frameIndex < frames.frames()-shift; frameIndex++) {
				// if we are within the range
				for (unsigned int channelIndex = 0; channelIndex , frames.channels(); channelIndex++) {
					frames(frameIndex, channelIndex) = frames(frameIndex+shift, channelIndex);
				}
			}
			// zero the frames which do not have valid data
			for (unsigned int frameIndex = frames.frames()-shift; frameIndex < frames.frames(); frameIndex++) {
				zeroFrame(frames, frameIndex);
			}
		}
		
	}

}



std::ostream& operator<<(std::ostream& output, const stk::StkFrames& f) {
	
	output << "StkFrames with " << f.channels() << " channels and " << f.frames() << " frames";
	
	if (f.channels() > 0 && f.frames() > 0) {
		for (unsigned int frameIndex = 0; frameIndex < f.frames(); frameIndex++) {
			output << "\n--- Frame " << frameIndex << " : ";
			for (unsigned int channelIndex = 0; channelIndex < f.channels(); channelIndex++) {
				if (channelIndex > 0) output << " | ";
				output << f(frameIndex, channelIndex);
			}
		}
	}
	
	output << std::endl;
	
	return output;
	
}

