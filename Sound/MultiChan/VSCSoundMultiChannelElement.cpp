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

VSCSoundMultiChannelElement::VSCSoundMultiChannelElement() {
    
    // DO NOT CALL VIRTUAL METHODS IN CONSTRUCTOR !!!
    this->setNumberOfChannels(1);
	
}

VSCSoundMultiChannelElement::VSCSoundMultiChannelElement(unsigned int numberOfChannels) {
    
    // DO NOT CALL VIRTUAL METHODS IN CONSTRUCTOR !!!
    this->setNumberOfChannels(numberOfChannels);
	
}


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

void VSCSoundMultiChannelElement::setLinearGain(VSCSFloat g) {
    std::vector<VSCSFloat> linearGains(_numberOfChannels, g);
    this->setLinearGains(linearGains);
}

void VSCSoundMultiChannelElement::setDBGain(VSCSFloat g) {
    std::vector<VSCSFloat> dBGains(_numberOfChannels, g);
    this->setDBGains(dBGains);
}


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

void VSCSoundMultiChannelElement::lockChannels(bool _lock) {
    _lockNumberOfChannels = _lock;
}

bool VSCSoundMultiChannelElement::numberOfChannelsIsLocked(void) {
    return _lockNumberOfChannels;
}

double VSCSoundMultiChannelElement::getValueForParameterWithKey(VSCSParameter::Key key) {
	
	int i = VSCSParameter::channelIndexForKey(key);
	
	if (i < this->getNumberOfChannels() && i >= 0) {
		if (VSCSParameter::parameterIsLinearChannel(key)) 
			return _channelLinearGains[i];
		if (VSCSParameter::parameterIsDBChannel(key)) 
			return VSCSound::linearToDBGain(_channelLinearGains[i]);
	}
	
	assert(false);

	return 0.0;
}

void VSCSoundMultiChannelElement::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
	
}


