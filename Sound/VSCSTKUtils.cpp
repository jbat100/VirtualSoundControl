/*
 *  VSCSTKUtils.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 01/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSTKUtils.h"
#include <cassert>

namespace stk {
    
void averageFramesChannels(stk::StkFrames& frames, stk::StkFrames& averagedFrames) {
    
    /*
     *  resize averagedFramed with the same number of frames as frames
     */
    averagedFrames.resize(frames.frames(), 1);
    
    for (unsigned int frameIndex = 0; frameIndex < frames.frames(); frameIndex++) {
        stk::StkFloat average = 0.0;
        for (unsigned int channelIndex = 0; channelIndex < frames.channels(); channelIndex++) {
            average += frames(frameIndex, channelIndex);
        }
        average /= (VSCSFloat)frames.channels();
        averagedFrames[frameIndex] = average;
    }
    
}

void setFramesChannel(stk::StkFrames& targetFrames, stk::StkFrames& monoFrames, unsigned int channel) {
    
    std::assert(targetFrames.frames() == monoFrames.frames());
    std::assert(monoFrames.channels() == 1);
    
    for (unsigned int frameIndex = 0; frameIndex < targetFrames.frames(); frameIndex++) {
        targetFrames(frameIndex, channel) = monoFrames[frameIndex];
    }
    
}

}

