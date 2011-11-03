/*
 *  VSCSynthSourceSine.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSynthSourceFileLoop.h"
#include "VSCSound.h"

stk::FileLoop* VSCSynthSourceFileLoop::getStkFileLoop(void) {
    return &fileLoop;
}