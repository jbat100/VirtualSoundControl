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
	
	_parameterKeys.insert(VSCSParameter::KeyChannelAll);
    _parameterKeys.insert(VSCSParameter::KeyChannelDBAll);
	for (int i = 0; i < _numberOfChannels; i++) {
		// insert keys for dB AND liner gain values
        _parameterKeys.insert(VSCSParameter::keyForChannelIndex(i,true));
		_parameterKeys.insert(VSCSParameter::keyForChannelIndex(i,false));
    }
    
    
    // DO NOT CALL VIRTUAL METHODS IN CONSTRUCTOR !!!
    this->setLinearGain(1.0);
}

unsigned int VSCSoundMultiChannelElement::getNumberOfChannels(void) {
    return _numberOfChannels;
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
		sum += VSCSound::linearToDBGain(_channelLinearGains[i]);
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
    int i = VSCSParameter::channelIndexForKey(key, &isDB);
    
    if (i != VSCSParameter::kChannelNotFound) {
        
        assert(i < this->getNumberOfChannels());
        assert(i < _channelLinearGains.size());
        
		if (isDB) {
			return _channelLinearGains[i];
		}
        else {
            return VSCSound::linearToDBGain(_channelLinearGains[i]);
        }
        
    }
	
	/*
	 *	If we have got to here (we should not), then pass upwards (which should throw)
	 */
	return VSCSoundParameterizedElement::getValueForParameterWithKey(key);
}

void VSCSoundMultiChannelElement::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
	
    if (key == VSCSParameter::KeyChannelAll) {
        this->setLinearGain(value);
    }
    else if (key == VSCSParameter::KeyChannelDBAll) {
        this->setDBGain(value);
        
    }
    
    bool isDB = false;
    int i = VSCSParameter::channelIndexForKey(key, &isDB);
    
    if (i != VSCSParameter::kChannelNotFound) {
        assert(i < this->getNumberOfChannels());
        assert(i < _channelLinearGains.size());
        if (isDB) {
            _channelLinearGains[i] = VSCSound::dBToLinearGain(value);
        }
        else {
            _channelLinearGains[i] = value;
        }
    }
	
	/*
	 *	If we have got to here (we should not), then pass upwards (which should throw)
	 */
	VSCSoundParameterizedElement::setValueForParameterWithKey(value, key);
	
}


void VSCSoundMultiChannelElement::resetMultiChannelParameters(void) {
	
	std::set<VSCSParameter::Key> channelsKeys = VSCSParameter::keysForDomain(VSCSParameter::DomainChannels);
	_parameterKeys.erase(channelsKeys.begin(), channelsKeys.end());
    
}

