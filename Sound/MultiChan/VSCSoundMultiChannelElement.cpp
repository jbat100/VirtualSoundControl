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
    
    this->resetMultiChannelParameters();
	
	this->addParameterKey(VSCSParameter::KeyChannelAll);
    this->addParameterKey(VSCSParameter::KeyChannelDBAll);
	for (int i = 0; i < _numberOfChannels; i++) {
		// insert keys for dB AND liner gain values
        this->addParameterKey(VSCSParameter::sharedInstance().keyForChannelIndex(i,true));
		this->addParameterKey(VSCSParameter::sharedInstance().keyForChannelIndex(i,false));
    }
    
    // DO NOT CALL VIRTUAL METHODS IN CONSTRUCTOR !!!
    this->setValueForParameterWithKey(1.0, VSCSParameter::KeyChannelAll);
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
	
    if (key == VSCSParameter::KeyChannelAll) {
        return this->averageLinearGain();
    }
    else if (key == VSCSParameter::KeyChannelDBAll) {
        return this->averageDBGain();
    }
    
    bool isDB = false;
    int i = VSCSParameter::sharedInstance().channelIndexForKey(key, &isDB);
    
    if (i != VSCSParameter::kChannelNotFound) {
        
        assert(i < this->getNumberOfChannels());
        assert(i < _channelLinearGains.size());
        
		if (isDB) {
			return _channelLinearGains[i];
		}
        else {
            return VSCSParameter::sharedInstance().linearToDBGain(_channelLinearGains[i]);
        }
        
    }
	
	/*
	 *	If we have got to here (we should not), then pass upwards (which should throw)
	 */
	return VSCSoundParameterized::getValueForParameterWithKey(key);
}

void VSCSoundMultiChannelElement::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
	
	if (key == VSCSParameter::KeyChannelAll || key == VSCSParameter::KeyChannelDBAll) {
		VSCSFloat linearGain;
		if (key == VSCSParameter::KeyChannelAll) linearGain = value;
		else if (key == VSCSParameter::KeyChannelDBAll) linearGain = VSCSParameter::sharedInstance().dBToLinearGain(value);
		for (unsigned int i = 0; i < _numberOfChannels; i++) {
			_channelLinearGains[i] = value;
		}
	}

    
    bool isDB = false;
    int i = VSCSParameter::sharedInstance().channelIndexForKey(key, &isDB);
    
    if (i != VSCSParameter::kChannelNotFound) {
        assert(i < _numberOfChannels);
        assert(i < _channelLinearGains.size());
        if (isDB) {
            _channelLinearGains[i] = VSCSParameter::sharedInstance().dBToLinearGain(value);
        }
        else {
            _channelLinearGains[i] = value;
        }
    }
	
	/*
	 *	If we have got to here (we should not), then pass upwards (which should throw)
	 */
	VSCSoundParameterized::setValueForParameterWithKey(value, key);
	
}


void VSCSoundMultiChannelElement::resetMultiChannelParameters(void) {
	
	VSCSParameter::KeySet channelsKeys = VSCSParameter::sharedInstance().keysForDomain(VSCSParameter::DomainChannels);
    
    for (VSCSParameter::KeySet::iterator iter = channelsKeys.begin(); iter != channelsKeys.end(); iter++) {
        this->removeParameterKey(*iter);
    }
    
}


std::string VSCSoundMultiChannelElement::getValueForPropertyWithKey(VSCSProperty::Key key) {	
	return VSCSoundPropertized::getValueForPropertyWithKey(key);
}

void VSCSoundMultiChannelElement::setValueForPropertyWithKey(std::string value, VSCSProperty::Key key) {
	VSCSoundPropertized::setValueForPropertyWithKey(value, key);
}


