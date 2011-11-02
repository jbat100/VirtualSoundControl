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
#include "Generator.h"

#include "VSCSound.h"

namespace stk {
    
void averageFramesChannels(stk::StkFrames& frames, stk::StkFrames& averagedFrames);
void setFramesChannel(stk::StkFrames& targetFrames, stk::StkFrames& monoFrames, unsigned int channel);
    
}

#endif
