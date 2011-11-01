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

void VSCSynthSourceElement::setLinearGain(VSCSFloat g) {
	_linearGain = g;
}

VSCSFloat VSCSynthSourceElement::getLinearGet(void) const {
	return _linearGain;
}

void VSCSynthSourceElement::setDBGain(VSCSFloat g) {
	_linearGain =  0.1 * std::pow(10,g);
}

VSCSFloat VSCSynthSourceElement::getDBGain(void) const {
	return 10.0 * std::log10(_linearGain);
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
