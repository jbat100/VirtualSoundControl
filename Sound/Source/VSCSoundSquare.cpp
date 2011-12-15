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
}


stk::BlitSquare& VSCSoundSquare::getStkBlitSquare(void) {
    return blitSquare;
}

