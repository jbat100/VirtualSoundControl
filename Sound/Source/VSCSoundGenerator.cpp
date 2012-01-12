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

VSCSParameter::KeyList VSCSoundGenerator::getInterfaceKeyList(void) const {
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

void VSCSoundGenerator::setLinearGain(VSCSFloat lG) {
    linearGain = lG;
}

VSCSFloat VSCSoundGenerator::getLinearGain(void) const {
    return linearGain;
}

void VSCSoundGenerator::setDBGain(VSCSFloat dBG) {
    linearGain = VSCSParameter::sharedInstance().dBToLinear(dBG);
}

VSCSFloat VSCSoundGenerator::getDBGain(void) const {
    return VSCSParameter::sharedInstance().linearToDB(linearGain);
}

#pragma mark - Parameter Setter/Getter 


VSCSFloat VSCSoundGenerator::getValueForParameterWithKey(VSCSParameter::Key k) const {
    if (k.domain == VSCSParameter::DomainGain) {
        if (k.code == VSCSParameter::CodeDBGain) {
            return this->getDBGain();
        }
        else if (k.code == VSCSParameter::CodeGain) {
            return this->getLinearGain();
        }
    }
	return VSCSoundElement::getValueForParameterWithKey(k);
}

void VSCSoundGenerator::setValueForParameterWithKey(double val, VSCSParameter::Key k) {
    if (k.domain == VSCSParameter::DomainGain) {
        if (k.code == VSCSParameter::CodeDBGain) {
            this->setDBGain(val);
        }
        else if (k.code == VSCSParameter::CodeGain) {
            this->setLinearGain(val);
        }
    }
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



