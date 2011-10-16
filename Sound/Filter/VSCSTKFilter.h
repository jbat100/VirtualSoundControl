/*
 *  VSCSTKFilter.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_STK_FILTER_
#define _VSC_STK_FILTER_

#include "VSCFilter.h"
#include "Stk.h"
#include "Filter.h"

VSCFrequencyPoint calculateFrequencyPoint(std::vector<StkFloat>& b, std::vector<StkFloat> a, 
										  StkFloat samplingFrequency, StkFloat pointFrequency);

#endif