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
	
    _numberOfChannelsNeededForComputationFrames = 0;
	_tickCount = 0;
	_group = NULL;
	_isOn = true;
	
	_numberOfPastFrames = 0;
	_fillPastFrames = false;
	
}

#pragma mark - Parameter Setter/Getter 


VSCSFloat VSCSoundGenerator::getValueForParameterWithKey(VSCSParameter::Key k) {
	return VSCSoundMultiChannelElement::getValueForParameterWithKey(k);
}

void VSCSoundGenerator::setValueForParameterWithKey(double val, VSCSParameter::Key k) {
	VSCSoundMultiChannelElement::setValueForParameterWithKey(val, k);
}

#pragma mark - Property Setter/Getter 


#pragma mark - Sound Engine

void VSCSoundGenerator::setNumberOfChannelsNeededForComputationFrames(unsigned int n) {
    _numberOfChannelsNeededForComputationFrames = n;
}

unsigned int VSCSoundGenerator::numberOfChannelsNeededForComputationFrames(void) {
    return _numberOfChannelsNeededForComputationFrames;
}

void VSCSoundGenerator::processComputationFrames(unsigned int numberOfFrames) {
	if (_computationFrames.frames() < numberOfFrames || 
		_computationFrames.channels() != _numberOfChannelsNeededForComputationFrames)
		_computationFrames.resize(numberOfFrames, _numberOfChannelsNeededForComputationFrames);
}


void VSCSoundGenerator::setNumberOfChannels(unsigned int c) {
	VSCSoundMultiChannelElement::setNumberOfChannels(c);
	this->updateSoundEngine();
}

void VSCSoundGenerator::initialize(void) {
	this->updateSoundEngine();
}

void VSCSoundGenerator::updateSoundEngine(void) {
    
    /*
	 *	resize _computationFrames to have 1 channel (only need mono noise generation)
	 *	which will get spread to the (possibly) multi-channel VSCSoundGenerator
	 */
    unsigned int n = this->numberOfChannelsNeededForComputationFrames();
    if (_computationFrames.channels() != n) {
        _computationFrames.resize(_computationFrames.frames(), n);
    }
    
    
    this->setupPastFrames();
}

/*
 *  Frame debugging
 */
void VSCSoundGenerator::tracePastFrames(unsigned int numberOfPastFrames) {
    
    _numberOfPastFrames = numberOfPastFrames;
    
    this->setupPastFrames();
    
}

void VSCSoundGenerator::setupPastFrames(void) {
    
    if (_numberOfPastFrames == 0) {
        _fillPastFrames = false;
        _pastFrames.resize(0,0);
    }
    
    else {
        
        _fillPastFrames = true;
        _pastFrames.resize(_numberOfPastFrames, this->getNumberOfChannels());
    }
    
}

const stk::StkFrames& VSCSoundGenerator::getPastFrames(void) {
    return _pastFrames;
}

unsigned long long VSCSoundGenerator::getTickCount(void) {
    return _tickCount;
}

/*
 * TICK (CENTRAL PROCESSING METHOD)
 */

stk::StkFrames& VSCSoundGenerator::tick(stk::StkFrames& frames, unsigned int channel)
{
    
    _tickCount++;
	
	if (this->isOn() == false)
		return frames;
    
    unsigned int numberOfChannels = this->getNumberOfChannels();
	
    assert(frames.channels() == numberOfChannels);
    if(frames.channels() != numberOfChannels)
        frames.resize(frames.frames(), numberOfChannels);
    
    /*
     *  Process _computationFrames independantly of _numberOfChannels
     */
    this->processComputationFrames(frames.frames());
    
    /*
     *  If we have exactly the same number of channels the we just assign the data
     *  the = operator is overloaded so that the data from _computationFrames is copied over
     */
    if (frames.channels() == _computationFrames.channels()) {
        frames = _computationFrames;
    }
    
    /*
     *  If we have a different number of channels in the _computationFrames than _numberOfChannels
     *  then we average the _computationFrames channels into a single channel and set all 
     *  _numberOfChannels to this averaged frames
     */
    else {
        assert(_computationFrames.channels() > 0);
        if (_computationFrames.channels() > 1) {
            stk::averageFramesChannels(_computationFrames, _averageFrames);
            stk::setAllFramesChannelsWithMonoFrames(frames, _averageFrames);
        }
        else if (_computationFrames.channels() == 1) {
            stk::setAllFramesChannelsWithMonoFrames(frames, _computationFrames);
        }
    }
    
    const std::vector<VSCSFloat> channelLinearGains = this->getChannelLinearGains();
    
    /*
     *  Scale the output channels as specified by _channelLinearGains
     */
    for (unsigned int i; i < numberOfChannels; i++) {
        stk::scaleFramesChannel(frames, i, (stk::StkFloat)channelLinearGains[i]);
    }
	
	if (_fillPastFrames) {
		if (_pastFrames.channels() != numberOfChannels || _pastFrames.frames() != _numberOfPastFrames)
			_pastFrames.resize(_numberOfPastFrames, numberOfChannels);
		stk::shiftFrames(_pastFrames, frames.frames());
		stk::substituteFramesFromIndex(frames, _pastFrames, 0);
	}
	
    
    lastFrame_ = frames;
	return frames;
}

