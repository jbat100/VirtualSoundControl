//
//  VSCSynthSourceGenerator.cpp
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/2/11.
//  Copyright (c) 2011 NXP. All rights reserved.
//

#include <iostream>
#include "VSCSynthSourceGenerator.h"

VSCSynthSourceGenerator::VSCSynthSourceGenerator() {
	this->_numberOfChannelsNeededForComputationFrames = 0;
	this->_tickCount = 0
	this->_group = NULL;
	this->_isOn = true;
	
	unsigned int _numberOfPastFrames = 0;
	bool _fillPastFrames = false;
	
}

#pragma mark - Parameter Setter/Getter 


VSCSFloat VSCSynthSourceGenerator::getValueForParameterWithKey(VSCSParameter::Key k) {
	return VSCSoundMultiChannelElement::getValueForParameterWithKey(k);
}

void VSCSynthSourceGenerator::setValueForParameterWithKey(double val, VSCSParameter::Key k) {
	VSCSoundMultiChannelElement::setValueForParameterWithKey(val, k);
}

#pragma mark - Property Setter/Getter 


#pragma mark - Sound Engine

void VSCSynthSourceGenerator::processComputationFrames(unsigned int numberOfFrames) {
	if (_computationFrames.frames() < numberOfFrames || 
		_computationFrames.channels() != _numberOfChannelsNeededForComputationFrames)
		_computationFrames.resize(numberOfFrames, _numberOfChannelsNeededForComputationFrames);
}


void VSCSynthSourceGenerator::setNumberOfChannels(unsigned int c) {
	VSCSoundMultiChannelElement::setNumberOfChannels(c);
	this->updateSoundEngine();
}

void VSCSynthSourceGenerator::initialize(void) {
	this->updateSoundEngine();
}

void VSCSynthSourceGenerator::updateSoundEngine(void) {

}

stk::StkFrames& VSCSynthSourceGenerator::tick(stk::StkFrames& frames, unsigned int channel)
{
	
	if (this->isOn() == false)
		return;
	
    assert(frames.channels() == _numberOfChannels);
    if(frames.channels() != _numberOfChannels)
        frames.resize(frames.frames(), _numberOfChannels);
    
    /*
     *  Process _computationFrames independantly of _numberOfChannels
     */
    this->processComputationFrames();
    
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
    
    assert(_numberOfChannels == _channelLinearGains.size());
    
    /*
     *  Scale the output channels as specified by _channelLinearGains
     */
    for (unsigned int i; i < _numberOfChannels; i++) {
        stk::scaleFramesChannel(frames, i, (stk::StkFloat)_channelLinearGains[i]);
    }
	
	if (fillPastFrames) {
		if (_pastFrames.channels() != _numberOfChannels || _pastFrames.frames() != _numberOfPastFrames)
			_pastFrames.resize(_numberOfPastFrames, _numberOfChannels);
		stk::shiftFrames(_pastFrames, frames.frames());
		stk::substituteFramesFromIndex(frames, _pastFrames, 0);
	}
	
    
    lastFrame_ = frames;
	return frames;
}