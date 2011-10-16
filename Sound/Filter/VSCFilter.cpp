/*
 *  VSCFilter.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCFilter.h"

#include <list>

ConstFreqPntIter VSCFilter::getFrequencyPointBeginConstIterator(void) const {
	return _frequencyPoints.begin();
}

ConstFreqPntIter VSCFilter::getFrequencyPointEndConstIterator(void) const {
	return _frequencyPoints.end();
}

void VSCFilter::setBypass(bool b) {
	
}

bool VSCFilter::isBypassed(void) {
	
}
