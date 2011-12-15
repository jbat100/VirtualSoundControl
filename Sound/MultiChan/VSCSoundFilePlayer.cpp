/*
 *  VSCSoundSine.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundFilePlayer.h"


stk::FileWvIn* VSCSoundFilePlayer::getStkFileWvIn(void) {
    return &fileWvIn;
}

