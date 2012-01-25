/*
 *  VSCSoundSine.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundFileLoopPlayer.h"
#include "VSCSound.h"

stk::FileLoop* VSCSoundFileLoopPlayer::getStkFileLoop(void) {
    return &fileLoop;
}