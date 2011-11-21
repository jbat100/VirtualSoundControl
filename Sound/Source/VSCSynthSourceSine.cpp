/*
 *  VSCSynthSourceSine.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSynthSourceSine.h"
#include "VSCSound.h"

VSCSynthSourceSine::VSCSynthSourceSine() {
	
	this->setFrequency(VSCSound::getReferenceAFrequency());
	// this->setPhase(0.0); // no need to do that...
	
}

double VSCSynthSourceSine::getValueForParameterWithKey(VSCSParameter::Key key) {
	
	if (key == VSCSParameter::KeySineFrequency) {
		return this->getFrequency();
	}
	if (key == VSCSParameter::KeySinePhase) {
		return this->getPhase();
	}
	
	//VSCSynthSourceGenerator::getValueForParameterWithKey(key);
	
	/* 
	 *	to save cycles call directly on VSCSoundMultiChannelElement which
	 *	is the next base class up the hierarchy to do somthing useful, 
	 *	this may change in future, so stay aware of this ...
	 */
	
	VSCSoundMultiChannelElement::getValueForParameterWithKey(key);
}

void VSCSynthSourceSine::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
	
	if (key == VSCSParameter::KeySineFrequency) {
		return this->setFrequency((VSCSFloat)value);
	}
	if (key == VSCSParameter::KeySinePhase) {
		return this->setPhase((VSCSFloat)value);
	}
	
	//VSCSynthSourceGenerator::getValueForParameterWithKey(key);
	
	/* 
	 *	to save cycles call directly on VSCSoundMultiChannelElement which
	 *	is the next base class up the hierarchy to do somthing useful, 
	 *	this may change in future, so stay aware of this ...
	 */
	
	VSCSoundMultiChannelElement::setValueForParameterWithKey(value, key);
	
}


stk::SineWave* VSCSynthSourceSine::getStkSineWave(void) {
    return &sineWave;
}

void VSCSynthSourceSine::updateSoundEngine(void) {
	
	// call superclass implementation
	VSCSynthSourceGenerator::updateSoundEngine();
	
	/*
	 *	resize _computationFrames to have 1 channel (only need mono noise generation)
	 *	which will get spread to the (possibly) multi-channel VSCSynthSourceGenerator
	 */
    if (_computationFrames.channels() != 1) {
        _computationFrames.resize(_computationFrames.frames(), 1);
    }
	
}

VSCSFloat VSCSynthSourceSine::getFrequency(void) {
	return _frequency;
}


void VSCSynthSourceSine::setFrequency(VSCSFloat f) {
	sineWave.setFrequency(f);
	_frequency = f;
}

VSCSFloat VSCSynthSourceSine::getPhase(void) {
	return _phase;
}

void VSCSynthSourceSine::setPhase(VSCSFloat f) {
	sineWave.addPhase(f-_phase);
	_phase -= f;
}

void VSCSynthSourceSine::addPhase(VSCSFloat f) {
	sineWave.addPhase(f);
	_phase += f;
}	
