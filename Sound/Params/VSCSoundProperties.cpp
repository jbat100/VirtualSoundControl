/*
 *  VSCSoundProperties.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundProperties.h"

VSCSProperty::VSCSProperty() {
	
	this->generatePropertyLabels();
	
}

VSCSProperty::~VSCSProperty() {
	
}

VSCSProperty& VSCSProperty::sharedInstance(void) {
	static VSCSProperty singletonInstance;
	return singletonInstance;
}


std::string VSCSProperty::getLabelForPropertyWithKey(VSCSProperty::Key k) {
	
	KeyLabelMap::iterator labelIterator = keyLabelMap.find(k);
	
	if (labelIterator != keyLabelMap.end()) 
		return labelIterator->second;
	
	throw VSCSBadPropertyException();
	
}

void VSCSProperty::setLabelForPropertyWithKey(std::string label, VSCSProperty::Key k) {
	
	KeyLabelMap::iterator labelIterator = keyLabelMap.find(k);
	
	if (labelIterator != keyLabelMap.end()) {
		keyLabelMap.erase(k);
	}
		
	keyLabelMap.insert(KeyLabelPair (k, label));
	
}


void VSCSProperty::generatePropertyLabels(void) {
	
	keyLabelMap.insert(KeyLabelPair (KeyNone, "No Property"));
	keyLabelMap.insert(KeyLabelPair (KeySoundElementType, "Sound Element Type"));
	keyLabelMap.insert(KeyLabelPair (KeyChannelSetup, "Channel Setup"));
	keyLabelMap.insert(KeyLabelPair (KeyFilePath, "File Path"));
	keyLabelMap.insert(KeyLabelPair (KeyBiQuadType, "BiQuad Filter Type"));
	keyLabelMap.insert(KeyLabelPair (KeyAll, "All Properties"));

}