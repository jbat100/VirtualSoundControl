/*
 *  VSCSoundParameterizedElement.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundParameterizedElement.h"
#include "VSCException.h"

std::set<VSCSParameter::Key>& VSCSoundParameterizedElement::getParameterKeys(void) {
	return _parameterKeys;
}

void VSCSoundParameterizedElement::addParameterKey(VSCSParameter::Key k) {
	this->getParameterKeys().insert(k);	
}

void VSCSoundParameterizedElement::removeParameterKey(VSCSParameter::Key k) {
    this->getParameterKeys().erase(k);	
}

/*
 *	If this class gets these messages then all subclasses have failed to deal with it
 *	so throw...
 */
double VSCSoundParameterizedElement::getValueForParameterWithKey(VSCSParameter::Key key) {
	throw VSCSBadParameterException();
}

void VSCSoundParameterizedElement::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
	throw VSCSBadParameterException();
}