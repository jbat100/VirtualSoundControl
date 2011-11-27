/*
 *  VSCSoundParameterizedElement.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundParameterized.h"
#include "VSCException.h"

std::set<VSCSParameter::Key>& VSCSoundParameterized::getParameterKeys(void) {
	return _parameterKeys;
}

void VSCSoundParameterized::addParameterKey(VSCSParameter::Key k) {
	this->getParameterKeys().insert(k);	
}

void VSCSoundParameterized::removeParameterKey(VSCSParameter::Key k) {
    this->getParameterKeys().erase(k);	
}

/*
 *	If this class gets these messages then all subclasses have failed to deal with it
 *	so throw...
 */
double VSCSoundParameterized::getValueForParameterWithKey(VSCSParameter::Key key) {
	throw VSCSBadParameterException();
}

void VSCSoundParameterized::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
	throw VSCSBadParameterException();
}


