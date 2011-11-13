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

extern const std::string kVSCSoundFrequencyParameterKey = "kVSCSoundFrequencyParameterKey";
extern const std::string kVSCSoundPhaseParameterKey = "kVSCSoundPhaseParameterKey";
extern const std::string kVSCSoundDBGainParameterKey = "kVSCSoundDBGainParameterKey";
extern const std::string kVSCSoundLinearGainParameterKey = "kVSCSoundLinearGainParameterKey";
extern const std::string kVSCSoundHarmonicsParameterKey = "kVSCSoundHarmonicsParameterKey";


void VSCSynthSourceElement::setOn(bool on) {
	_isOn = on;
}

bool VSCSynthSourceElement::isOn(void) const {
	return _isOn;
}

std::string VSCSynthSourceElement::sourceTypeString(void) {
    static std::string sourceType = "base source element";
	return sourceType;
}



/*
 *  Perform initialization after constructor (seperate so that it can be virtual).
 */
void VSCSynthSourceElement::initialize(void) {
	
}

/*
 *  Perform necessary setup operations after:
 *      - a change in number of channels.
 */
void VSCSynthSourceElement::updateSoundEngine(void) {
	
}
