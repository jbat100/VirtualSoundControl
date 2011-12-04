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

VSCSParameter::KeySet VSCSoundParameterized::getKeySet(void) {
    return VSCSParameter::KeySet(); // return empty key set
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

double VSCSoundParameterized::getValueForIndexedParameterWithKey(VSCSParameter::Key key, unsigned int ind) {
	throw VSCSBadParameterException();
}

void VSCSoundParameterized::setValueForIndexedParameterWithKey(double value, VSCSParameter::Key key, unsigned int ind) {
	throw VSCSBadParameterException();
}

