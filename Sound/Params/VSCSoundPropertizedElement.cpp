/*
 *  VSCSoundPropertizedElement.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundPropertizedElement.h"



std::set<VSCSProperty::Key>& VSCSoundPropertizedElement::getPropertyKeys(void) {
	return _propertyKeys;
}

void VSCSoundPropertizedElement::addPropertyKey(VSCSProperty::Key k) {
	this->getPropertyKeys().insert(k);	
}

void VSCSoundPropertizedElement::removePropertyKey(VSCSProperty::Key k) {
    this->getPropertyKeys().erase(k);	
}