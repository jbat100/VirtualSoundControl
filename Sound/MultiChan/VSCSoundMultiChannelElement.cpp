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

VSCSoundMultiChannelElement::VSCSoundMultiChannelElement() {
    
    // DO NOT CALL VIRTUAL METHODS IN CONSTRUCTOR !!!
    this->setNumberOfChannels(1);
	
}

VSCSoundMultiChannelElement::VSCSoundMultiChannelElement(unsigned int numberOfChannels) {
    
    // DO NOT CALL VIRTUAL METHODS IN CONSTRUCTOR !!!
    this->setNumberOfChannels(numberOfChannels);
	
}

#pragma mark Number Of Channels Setter/Getter

void VSCSoundMultiChannelElement::setNumberOfChannels(unsigned int numberOfChannels) {
    
    if (this->numberOfChannelsIsLocked())
        return;
    
    assert(numberOfChannels < kVSCSMaxChannels);
    if (numberOfChannels > kVSCSMaxChannels)
        numberOfChannels = kVSCSMaxChannels;
    _numberOfChannels = numberOfChannels;
    _channelLinearGains.resize(_numberOfChannels);
    
    // DO NOT CALL VIRTUAL METHODS IN CONSTRUCTOR !!!
    this->setLinearGain(1.0);
}

unsigned int VSCSoundMultiChannelElement::getNumberOfChannels(void) {
    return _numberOfChannels;
}

#pragma mark General Gain Setter

void VSCSoundMultiChannelElement::setLinearGain(VSCSFloat g) {
    std::vector<VSCSFloat> linearGains(_numberOfChannels, g);
    this->setLinearGains(linearGains);
}

void VSCSoundMultiChannelElement::setDBGain(VSCSFloat g) {
    std::vector<VSCSFloat> dBGains(_numberOfChannels, g);
    this->setDBGains(dBGains);
}

#pragma mark Vector Gain Setter/Getters

void VSCSoundMultiChannelElement::setLinearGains(std::vector<VSCSFloat>& channelGains) {
    assert(channelGains.size() == _numberOfChannels);
    assert(_channelLinearGains.size() == _numberOfChannels);
    std::copy(channelGains.begin(), channelGains.end(), _channelLinearGains.begin());
}

void VSCSoundMultiChannelElement::getLinearGains(std::vector<VSCSFloat>& channelGains) const {
    channelGains.resize(_numberOfChannels);
    assert(_channelLinearGains.size() == _numberOfChannels);
    std::copy(_channelLinearGains.begin(), _channelLinearGains.end(), channelGains.begin());
}

void VSCSoundMultiChannelElement::setDBGains(std::vector<VSCSFloat>& channelDBGains) {
    assert(channelDBGains.size() == _numberOfChannels);
    std::vector<VSCSFloat> channelGains(_numberOfChannels);
    for (unsigned int i = 0; i < _numberOfChannels; i++) 
        channelGains[i] = VSCSound::dBToLinearGain(channelDBGains[i]);
    this->setLinearGains(channelGains);
}

void VSCSoundMultiChannelElement::getDBGains(std::vector<VSCSFloat>& channelDBGains) const {
    channelDBGains.resize(_numberOfChannels);
    assert(_channelLinearGains.size() == _numberOfChannels);
    for (unsigned int i = 0; i < _numberOfChannels; i++) 
        channelDBGains[i] = VSCSound::linearToDBGain(_channelLinearGains[i]);
}

VSCSFloat VSCSoundMultiChannelElement::averageLinearGain(void) {
	VSCSFloat sum = 0.0;
	for (int i = 0; i<_numberOfChannels; i++) {
		sum += _channelLinearGains[i];
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

bool isMultiChannelElementParameterKey(VSCSParameter::Key k) {
	
}

bool isMultiChannelElementPropertyKey(VSCSProperty::Key k) {
	
}

double VSCSoundMultiChannelElement::getValueForParameterWithKey(VSCSParameter::Key key) {
	
	if (VSCSParameter::parameterIsLinearChannel(key)) {
		if (key == VSCSParameter::KeyChannelAll) {
			return this->averageLinearGain();
		}
		else {
			int i = VSCSParameter::channelIndexForKey(key);
			assert(i < this->getNumberOfChannels());
			assert(i < _channelLinearGains.size());
			return _channelLinearGains[i];
		}
	}
	
	
	if (VSCSParameter::parameterIsDBChannel(key)) {
		if (key == VSCSParameter::KeyChannelDBAll) {
			return VSCSound::linearToDBGain(this->averageLinearGain());
		}
		else {
			int i = VSCSParameter::channelIndexForKey(key);
			assert(i < this->getNumberOfChannels());
			assert(i < _channelLinearGains.size());
			return VSCSound::linearToDBGain(_channelLinearGains[i]);
		}
	}
	
	/*
	 *	If we have got to here (we should not), then pass upwards (which should throw)
	 */
	VSCSoundPropertizedElement::getValueForParameterWithKey(key);

	return 0.0;
}

void VSCSoundMultiChannelElement::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
	
	if (VSCSParameter::parameterIsLinearChannel(key)) {
		if (key == VSCSParameter::KeyChannelAll) {
			this->setLinearGain(value);
		}
		else {
			int i = VSCSParameter::channelIndexForKey(key);
			assert(i < this->getNumberOfChannels());
			assert(i < _channelLinearGains.size());
			_channelLinearGains[i] = value;
		}
	}
	
	else if (VSCSParameter::parameterIsDBChannel(key)) {
		if (key == VSCSParameter::KeyChannelDBAll) {
			this->setDBGain(value);
		}
		else {
			int i = VSCSParameter::channelIndexForKey(key);
			assert(i < this->getNumberOfChannels());
			assert(i < _channelLinearGains.size());
			_channelLinearGains[i] = VSCSound::dBToLinearGain(value);
		}
	}
	
	/*
	 *	If we have got to here (we should not), then pass upwards (which should throw)
	 */
	VSCSoundPropertizedElement::setValueForParameterWithKey(value, key);
	
}


double VSCSoundMultiChannelElement::getValueForPropertyWithKey(VSCSProperty::Key key) {
	
}

void VSCSoundMultiChannelElement::setValueForPropertyWithKey(double value, VSCSProperty::Key key) {
	
}

