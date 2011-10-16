//
//  VSCSynthSourceElement.cpp
//  SynthStation
//
//  Created by Jonathan Thorpe on 9/26/11.
//  Copyright 2011 NXP. All rights reserved.
//

#include <iostream>
#include "VSCSynthSourceElement.h"
#include <cmath>

void VSCSynthSourceElement::setLinearGain(double g) {
	_linearGain = g;
}

double VSCSynthSourceElement::getLinearGet(void) {
	return _linearGain;
}

void VSCSynthSourceElement::setDBGain(double g) {
	_linearGain =  0.1 * std::pow(10,g);
}

void VSCSynthSourceElement::getDBGain(void) {
	return 10.0 * std::log10(_linearGain);
}

void VSCSynthSourceElement::setOn(bool on) {
	_isOn = on;
}

bool VSCSynthSourceElement::isOn(void) {
	return _isOn;
}

