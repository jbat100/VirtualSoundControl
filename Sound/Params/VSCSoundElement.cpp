/*
 *  VSCSoundElement.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 27/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundElement.h"

#include <boost/lexical_cast.hpp>

unsigned int VSCSoundElement::elementCount = 0;

VSCSoundElement::VSCSoundElement() {
	elementIdentifier = "VSCSElement_" + boost::lexical_cast<std::string>(VSCSoundElement::elementCount);
	VSCSoundElement::elementCount++;
}

std::string VSCSoundElement::getElementIdentifier(void) {
	return elementIdentifier;
}

std::string VSCSoundElement::getElementType(void) {
	static const std::string t("VSCSoundElement");
	return t;
}

