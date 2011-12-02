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

const std::string VSCSoundElement::kSoundElementType = "VSCSoundElement";

VSCSoundElement::VSCSoundElement() {
    elementId = VSCSoundElement::elementCount;
	VSCSoundElement::elementCount++;
}

std::string VSCSoundElement::getElementIdentifier(void) {
	return 	this->getElementType() + "_" + boost::lexical_cast<std::string>(VSCSoundElement::elementCount);;
}

std::string VSCSoundElement::getElementType(void) {
	return VSCSoundElement::kSoundElementType;
}
