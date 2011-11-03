/*
 *  VSCSynthSourceSine.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSynthSourceNoise.h"


stk::Noise* VSCSynthSourceNoise::getStkNoise(void) {
    return &noise;
}