/*
 *  VSCFrequencyPoint.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 22/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_FREQUENCY_POINT_
#define _VSC_FREQUENCY_POINT_

#include "VSCSound.h"

typedef struct VSCFrequencyPoint {
	VSCSFloat frequency;
	VSCSFloat dB;
	VSCSFloat phase;
} VSCFrequencyPoint;

#endif