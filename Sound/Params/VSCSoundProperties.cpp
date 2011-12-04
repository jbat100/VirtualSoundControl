/*
 *  VSCSoundProperties.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundProperties.h"
#include "VSCException.h"

bool VSCSProperty::Key::operator<(const Key& otherKey) const {
	if (domain < otherKey.domain)
		return true;
	if (code < otherKey.code)
		return true;
	return false;
}


VSCSProperty::VSCSProperty() {
	
}

VSCSProperty::~VSCSProperty() {
	
}

VSCSProperty& VSCSProperty::sharedInstance(void) {
	static VSCSProperty singletonInstance;
	return singletonInstance;
}


std::string VSCSProperty::getLabelForPropertyWithKey(VSCSProperty::Key k) {
	
	KeyLabelMap::iterator labelIterator = customizedKeyLabels.find(k);
	if (labelIterator != customizedKeyLabels.end()) 
		return labelIterator->second;
	
	switch (k.code) {
		case CodeNone:
			return "No Code";
		case CodeSoundElementType:
			return "Element Type";
		case CodeSoundElementIdentifier:
			return "Element Identifier";
		case CodeChannelSetup:
			return "Channel Setup";
		case CodeFilePath:
			return "File Path";
		case CodeBiQuadType:
			return "BiQuad Type";
		case CodeAll:
			return "All Codes";
		default:
			break;
	}
	
	throw VSCSBadPropertyException();
	
}

void VSCSProperty::setLabelForPropertyWithKey(std::string label, VSCSProperty::Key k) {
	
	KeyLabelMap::iterator labelIterator = customizedKeyLabels.find(k);
	
	if (labelIterator != customizedKeyLabels.end()) {
		customizedKeyLabels.erase(k);
	}
		
	customizedKeyLabels.insert(KeyLabelPair (k, label));
	
}
