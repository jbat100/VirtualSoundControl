/*
 *  VSCSoundSine.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundSine.h"
#include "VSCSound.h"
#include "VSCMath.h"

const std::string VSCSoundSine::kSynthSourceSineType = "VSCSoundSine";

VSCSoundSine::VSCSoundSine() {
	this->setFrequency(VSCSound::getReferenceAFrequency());
	this->setPhase(0.0); // no need to do that...
}

VSCSParameter::KeyList VSCSoundSine::getInterfaceKeyList(void) const {
	VSCSParameter::KeyList keyList = VSCSoundGenerator::getInterfaceKeyList();
	keyList.push_back((VSCSParameter::Key){VSCSParameter::DomainSourceSine, VSCSParameter::CodeLogFrequency, 0});
	keyList.push_back((VSCSParameter::Key){VSCSParameter::DomainSourceSine, VSCSParameter::CodeRadPhase, 0});
    std::cout << "\nCreated list with " << keyList.size( ) << " elements" << std::endl; 
	return keyList;
}

VSCSFloat VSCSoundSine::tick(void) {
	VSCSFloat val;
#ifdef VSCS_USE_STK
	val = sineWave.tick();
#endif
#ifdef VSCS_DEBUG
	this->trace(val);
#endif
	return val;
}

double VSCSoundSine::getValueForParameterWithKey(VSCSParameter::Key key) const {
	if (key.domain == VSCSParameter::DomainSourceSine) {
		if (key.code == VSCSParameter::CodeFrequency) {
			return this->getFrequency();
		}
        if (key.code == VSCSParameter::CodeLogFrequency) {
			return VSCSound::frequencyToLogFrequency(this->getFrequency());
		}
		else if (key.code == VSCSParameter::CodeRadPhase) {
			return this->getPhase();
		}
	}
	return VSCSoundGenerator::getValueForParameterWithKey(key);
}

void VSCSoundSine::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
	if (key.domain == VSCSParameter::DomainSourceSine) {
		if (key.code == VSCSParameter::CodeFrequency) {
			this->setFrequency((VSCSFloat)value);
			return;
		}
		if (key.code == VSCSParameter::CodeRadPhase) {
			this->setPhase((VSCSFloat)value);
			return;
		}
	}
	VSCSoundGenerator::setValueForParameterWithKey(value, key);
}


const stk::SineWave& VSCSoundSine::getStkSineWave(void) {
    return sineWave;
}

VSCSFloat VSCSoundSine::getFrequency(void) const {
	return _frequency;
}

void VSCSoundSine::setFrequency(VSCSFloat f) {
	sineWave.setFrequency(f);
	_frequency = f;
}

VSCSFloat VSCSoundSine::getPhase(void) const {
	return _phase;
}

void VSCSoundSine::setPhase(VSCSFloat f) {
	sineWave.addPhase(f-_phase);
	_phase -= f;
}

void VSCSoundSine::addPhase(VSCSFloat f) {
	sineWave.addPhase(vsc::radiansToNormalized(f));
	_phase += f;
}	
