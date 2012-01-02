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

VSCSParameter::KeyList VSCSoundGenerator::getInterfaceKeyList(void) {
	VSCSParameter::KeyList keyList = VSCSoundElement::getInterfaceKeyList();
	VSCSParameter::Key k = {VSCSParameter::DomainGain, VSCSParameter::CodeDBGain, 0};
	keyList.push_back(k);
    return keyList;
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
void VSCSoundGenerator::tracePastSamples(unsigned int n) {
    _traceSampleSize = n;
}
unsigned int VSCSoundGenerator::traceSampleSize(void) {
	return _traceSampleSize;
}
const std::deque<VSCSFloat>& VSCSoundGenerator::getPastSamples(void) {
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
	while (_pastSamples.size() > _traceSampleSize) {
		_pastSamples.pop_front();
	}
}
#endif // VSCS_DEBUG



