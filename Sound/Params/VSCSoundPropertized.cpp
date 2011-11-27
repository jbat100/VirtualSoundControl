/*
 *  VSCSoundPropertizedElement.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundPropertized.h"

#include "VSCException.h"


std::set<VSCSProperty::Key>& VSCSoundPropertized::getPropertyKeys(void) {
	return _propertyKeys;
}

void VSCSoundPropertized::addPropertyKey(VSCSProperty::Key k) {
	this->getPropertyKeys().insert(k);	
}

void VSCSoundPropertized::removePropertyKey(VSCSProperty::Key k) {
    this->getPropertyKeys().erase(k);	
}


double VSCSoundPropertized::getValueForPropertyWithKey(VSCSProperty::Key key) {
	throw VSCSBadPropertyException();
}

void VSCSoundPropertized::setValueForPropertyWithKey(double value, VSCSProperty::Key key) {
	throw VSCSBadPropertyException();
}