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


VSCSProperty::KeySet VSCSoundPropertized::getPropertyKeys(void) {
	return VSCSProperty::KeySet();
}

std::string VSCSoundPropertized::getValueForPropertyWithKey(VSCSProperty::Key key) const {
	throw VSCSBadPropertyException();
}

void VSCSoundPropertized::setValueForPropertyWithKey(std::string value, VSCSProperty::Key key) {
	throw VSCSBadPropertyException();
}