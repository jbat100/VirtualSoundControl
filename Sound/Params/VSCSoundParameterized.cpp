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

VSCSParameter::KeyList VSCSoundParameterized::getInterfaceKeyList(void) {
    return VSCSParameter::KeyList(); // return empty key set
}

/*
 *	If this class gets these messages then all subclasses have failed to deal with it
 *	so throw...
 */

double VSCSoundParameterized::getValueForParameterWithKey(VSCSParameter::Key key) const {
    std::cout << "BAD PARAMETER KEY " << VSCSParameter::sharedInstance().getLabelForParameterWithKey(key) << std::endl;
    throw VSCSBadParameterException();
}

void VSCSoundParameterized::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
    std::cout << "BAD PARAMETER KEY " << VSCSParameter::sharedInstance().getLabelForParameterWithKey(key) << std::endl;
	throw VSCSBadParameterException();
}

double VSCSoundParameterized::getValueForIndexedParameterWithKey(VSCSParameter::Key key, unsigned int ind) const {
    std::cout << "BAD PARAMETER KEY " << VSCSParameter::sharedInstance().getLabelForParameterWithKey(key) << std::endl;
	throw VSCSBadParameterException();
}

void VSCSoundParameterized::setValueForIndexedParameterWithKey(double value, VSCSParameter::Key key, unsigned int ind) {
    std::cout << "BAD PARAMETER KEY " << VSCSParameter::sharedInstance().getLabelForParameterWithKey(key) << std::endl;
	throw VSCSBadParameterException();
}

