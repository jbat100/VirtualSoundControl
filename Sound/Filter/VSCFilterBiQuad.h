/*
 *  VSCFilterBiQuad.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_FILTER_BIQUAD_
#define _VSC_FILTER_BIQUAD_

#include "VSCFilter.h"
#include "BiQuad.h"

#include <list>


class VSCFilterBiQuad : VSCFilter, stk::BiQuad  {
	
public:
	
	void updateFrequencyPoints(void);
	
protected:
	
	
};


#endif