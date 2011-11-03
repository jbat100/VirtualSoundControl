/*
 *  VSCSTKUtils.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 01/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_STK_UTILS_H_
#define _VSC_STK_UTILS_H_

#include "Stk.h"

namespace stk {
    
	void averageFramesChannels(StkFrames& frames, StkFrames& averagedFrames);
	void setFramesChannelWithMonoFrames(StkFrames& targetFrames, StkFrames& monoFrames, unsigned int channel);
    void setAllFramesChannelsWithMonoFrames(StkFrames& targetFrames, StkFrames& monoFrames);
	void scaleFramesChannel(StkFrames& targetFrames, unsigned int channel, StkFloat scale);
    void zeroFrames(StkFrames& targetFrames);
    
}

#endif
