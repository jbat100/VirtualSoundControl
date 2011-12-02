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
	
	this->setNumberOfChannelsNeededForComputationFrames(1);
	this->setFrequency(VSCSound::getReferenceAFrequency());
	this->setPhase(0.0); // no need to do that...
    
}

void VSCSynthSourceSine::processComputationFrames(unsigned int numberOfFrames) {
	//VSCSynthSourceGenerator::processComputationFrames(numberOfFrames);
	sineWave.tick(_computationFrames);
}

double VSCSynthSourceSine::getValueForParameterWithKey(VSCSParameter::Key key) {
	
	if (key == VSCSParameter::KeySineFrequency) {
		return this->getFrequency();
	}
	if (key == VSCSParameter::KeySinePhase) {
		return this->getPhase();
	}
	
	return VSCSynthSourceGenerator::getValueForParameterWithKey(key);
}

void VSCSynthSourceSine::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
	
	if (key == VSCSParameter::KeySineFrequency) {
		this->setFrequency((VSCSFloat)value);
        return;
	}
	if (key == VSCSParameter::KeySinePhase) {
		this->setPhase((VSCSFloat)value);
        return;
	}
	
	VSCSynthSourceGenerator::setValueForParameterWithKey(value, key);
	
}


const stk::SineWave& VSCSynthSourceSine::getStkSineWave(void) {
    return sineWave;
}

void VSCSynthSourceSine::updateSoundEngine(void) {
	
	// call superclass implementation
	VSCSynthSourceGenerator::updateSoundEngine();
	
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
