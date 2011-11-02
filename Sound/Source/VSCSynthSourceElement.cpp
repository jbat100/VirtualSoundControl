//
//  VSCSynthSourceElement.cpp
//  SynthStation
//
//  Created by Jonathan Thorpe on 9/26/11.
//  Copyright 2011 NXP. All rights reserved.
//

#include <iostream>
#include "VSCSynthSourceElement.h"
#include "VSCSound.h"

#include <cmath>
#include <algorithm>

void VSCSynthSourceElement::setNumberOfChannels(unsigned int numberOfChannels) {
    _numberOfChannels = numberOfChannels;
    _channelLinearGains.resize(_numberOfChannels);
}

unsigned int VSCSynthSourceElement::getNumberOfChannels(void) {
    return _numberOfChannels;
}

void VSCSynthSourceElement::setLinearGain(VSCSFloat g) {
    std::vector<VSCSFloat> linearGains(_numberOfChannels, g);
    setLinearGains(linearGains);
}

void VSCSynthSourceElement::setDBGain(VSCSFloat g) {
    std::vector<VSCSFloat> dBGains(_numberOfChannels, g);
    setDBGains(dBGains);
}


void VSCSynthSourceElement::setLinearGains(std::vector<VSCSFloat>& channelGains) {
    assert(channelGains.size() == _numberOfChannels);
    assert(_channelLinearGains.size() == _numberOfChannels);
    std::copy(channelGains.begin(), channelGains.end(), _channelLinearGains.begin());
}

void VSCSynthSourceElement::getLinearGains(std::vector<VSCSFloat>& channelGains) const {
    channelGains.resize(_numberOfChannels);
    assert(_channelLinearGains.size() == _numberOfChannels);
    std::copy(_channelLinearGains.begin(), _channelLinearGains.end(), channelGains.begin());
}

void VSCSynthSourceElement::setDBGains(std::vector<VSCSFloat>& channelDBGains) {
    assert(channelDBGains.size() == _numberOfChannels);
    std::vector<VSCSFloat> channelGains(_numberOfChannels);
    for (unsigned int i = 0; i < _numberOfChannels; i++) 
        channelGains[i] = dBToLinearGain(channelDBGains[i]);
    setLinearGains(channelGains);
}

void VSCSynthSourceElement::getDBGains(std::vector<VSCSFloat>& channelDBGains) const {
    channelDBGains.resize(_numberOfChannels);
    assert(_channelLinearGains.size() == _numberOfChannels);
    for (unsigned int i = 0; i < _numberOfChannels; i++) 
        channelDBGains[i] = linearToDBGain(_channelLinearGains[i]);
}

void VSCSynthSourceElement::setOn(bool on) {
	_isOn = on;
}

bool VSCSynthSourceElement::isOn(void) const {
	return _isOn;
}

std::string VSCSynthSourceElement::sourceTypeString(void) {
	return "base source element";
}
