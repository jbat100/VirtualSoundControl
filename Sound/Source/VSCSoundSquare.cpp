/*
 *  VSCSoundSine.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundSquare.h"

VSCSoundSquare::VSCSoundSquare () {
	this->setNumberOfChannelsNeededForComputationFrames(1);
}


stk::BlitSquare& VSCSoundSquare::getStkBlitSquare(void) {
    return blitSquare;
}

void VSCSoundSquare::processComputationFrames(void) {
	
}
