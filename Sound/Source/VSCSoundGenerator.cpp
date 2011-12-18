//
//  VSCSoundGenerator.cpp
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/2/11.
//  Copyright (c) 2011 NXP. All rights reserved.
//

#include <iostream>
#include "VSCSoundGenerator.h"

VSCSoundGenerator::VSCSoundGenerator() {
	_tickCount = 0;
	_group = NULL;
	_isOn = true;
	_traceSampleSize = 0;
}

void VSCSoundGenerator::setIsOn(bool o) {
	_isOn = o;
}

bool VSCSoundGenerator::isOn(void) {
	return _isOn;
}

#pragma mark - Parameter Setter/Getter 


VSCSFloat VSCSoundGenerator::getValueForParameterWithKey(VSCSParameter::Key k) {
	return VSCSoundElement::getValueForParameterWithKey(k);
}

void VSCSoundGenerator::setValueForParameterWithKey(double val, VSCSParameter::Key k) {
	VSCSoundElement::setValueForParameterWithKey(val, k);
}

#pragma mark - Audio Engine

void VSCSoundGenerator::initialize(void) {
	
}

#ifdef VSCS_DEBUG
/*
 *  Frame debugging
 */
void VSCSoundGenerator::tracePastFrames(unsigned int n) {
    _traceSampleSize = n;
}
unsigned int VSCSoundGenerator::traceSampleSize(void) {
	return _traceSampleSize;
}
const std::deque<stk::VSCSFloat>& VSCSoundGenerator::getPastSamples(void)(void) {
    return _pastSamples;
}
unsigned long long VSCSoundGenerator::getTickCount(void) {
    return _tickCount;
}
void VSCSoundGenerator::trace(VSCSFloat f) {
	_tickCount++;
	if (_traceSampleSize > 0) {
		_pastSamples.push_back(f);
	}
	while (_traceSampleSize.size() > _traceSampleSize) {
		_traceSampleSize.pop_front();
	}
}
#endif // VSCS_DEBUG



