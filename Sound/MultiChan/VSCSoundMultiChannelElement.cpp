/*
 *  VSCSoundMultiChannelElement.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 05/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundMultiChannelElement.h"
#include <cassert>

#pragma mark Constructor/Destructor

const std::string VSCSoundMultiChannelElement::kSoundMultiChannelType = "VSCSoundMultiChannelElement";

VSCSoundMultiChannelElement::VSCSoundMultiChannelElement() {
    
	_numberOfChannels = 0;
	_lockNumberOfChannels = false;
	
}


#pragma mark Number Of Channels Setter/Getter

void VSCSoundMultiChannelElement::setNumberOfChannels(unsigned int numberOfChannels) {
    
    if (this->numberOfChannelsIsLocked())
        return;
    
    assert(numberOfChannels <= kVSCSMaxChannels);
    if (numberOfChannels > kVSCSMaxChannels)
        numberOfChannels = kVSCSMaxChannels;
	
    _numberOfChannels = numberOfChannels;
    _channelLinearGains.resize(_numberOfChannels);
    
    // DO NOT CALL VIRTUAL METHODS IN CONSTRUCTOR !!!
	VSCSParameter::Key k =  {VSCSParameter::DomainChannel, VSCSParameter::CodeGain};
    this->setValueForParameterWithKey(1.0,k);
}

unsigned int VSCSoundMultiChannelElement::getNumberOfChannels(void) {
    return _numberOfChannels;
}

const std::vector<VSCSFloat>& VSCSoundMultiChannelElement::getChannelLinearGains(void) {
    return _channelLinearGains;
}

#pragma mark Average Gains

VSCSFloat VSCSoundMultiChannelElement::averageLinearGain(void) {
	VSCSFloat sum = 0.0;
	for (int i = 0; i<_numberOfChannels; i++) {
		sum += _channelLinearGains[i];
	}
	return sum/(VSCSFloat)_numberOfChannels;
}

VSCSFloat VSCSoundMultiChannelElement::averageDBGain(void) {
    VSCSFloat sum = 0.0;
	for (int i = 0; i<_numberOfChannels; i++) {
		sum += VSCSParameter::sharedInstance().linearToDBGain(_channelLinearGains[i]);
	}
	return sum/(VSCSFloat)_numberOfChannels;
}

#pragma mark Number Of Channel Locking

void VSCSoundMultiChannelElement::lockChannels(bool _lock) {
    _lockNumberOfChannels = _lock;
}

bool VSCSoundMultiChannelElement::numberOfChannelsIsLocked(void) {
    return _lockNumberOfChannels;
}

#pragma mark Parameters

double VSCSoundMultiChannelElement::getValueForParameterWithKey(VSCSParameter::Key key) {
    
	if (key.domain == VSCSParameter::DomainChannel) {
		if (key.code == VSCSParameter::CodeGain) {
			return this->averageLinearGain();
		}
	}
	
	return VSCSoundElement::getValueForParameterWithKey(key);
	
}

void VSCSoundMultiChannelElement::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
	
	if (key.domain == VSCSParameter::DomainChannel) {
		if (key.code == VSCSParameter::CodeGain) {
			for (unsigned int i = 0; i < _numberOfChannels; i++) {
				_channelLinearGains[i] = value;
			}
		}
	}
	
	VSCSoundElement::setValueForParameterWithKey(value, key);
	
}

double VSCSoundMultiChannelElement::getValueForIndexedParameterWithKey(VSCSParameter::Key key, unsigned int ind) {
	
	if (key.domain == VSCSParameter::DomainChannel) {
		if (ind < this->getNumberOfChannels()) {
			if (key.code == VSCSParameter::CodeGain) 
				return this->getChannelLinearGains()[ind];
			else 
				throw VSCSBadParameterException();
		}
		else {
			throw VSCSOutOfBoundsException();
		}
	}
	
	return VSCSoundElement::getValueForIndexedParameterWithKey(key, ind);
	
}

void VSCSoundMultiChannelElement::setValueForIndexedParameterWithKey(double value, VSCSParameter::Key key, unsigned int ind) {
	
	if (key.domain == VSCSParameter::DomainChannel) {
		if (ind < this->getNumberOfChannels()) {
			if (key.code == VSCSParameter::CodeGain) 
				_channelLinearGains[ind] = value;
			else 
				throw VSCSBadParameterException();
		}
		else {
			throw VSCSOutOfBoundsException();
		}
	}
	
	VSCSoundElement::getValueForIndexedParameterWithKey(key, ind);
	
}



std::string VSCSoundMultiChannelElement::getValueForPropertyWithKey(VSCSProperty::Key key) {	
	return VSCSoundPropertized::getValueForPropertyWithKey(key);
}

void VSCSoundMultiChannelElement::setValueForPropertyWithKey(std::string value, VSCSProperty::Key key) {
	VSCSoundPropertized::setValueForPropertyWithKey(value, key);
}


