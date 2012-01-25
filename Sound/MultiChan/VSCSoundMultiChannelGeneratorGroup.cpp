/*
 *  VSCSoundGeneratorGroup.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 01/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundGeneratorGroup.h"

#include "VSCSTKUtils.h"
#include <algorithm>


void VSCSoundGeneratorGroup::addGenerator(VSCSoundGeneratorPtr elem) {
	
	assert(elem);
	
	if (!elem)
		return;
	
	unsigned int numberOfChannels = this->getNumberOfChannels();
	
    assert(elem->getNumberOfChannels() == numberOfChannels);
    
    if (elem->getNumberOfChannels() != numberOfChannels) {
        assert(elem->numberOfChannelsIsLocked() == false);
        elem->setNumberOfChannels(numberOfChannels);
    }
    
	ConstSynthSrcGenIter it = find (_generators.begin(), _generators.end(), elem);
	
	assert(it == _generators.end());
	
	if (it == _generators.end()) {
		_generators.push_back(elem);
	}
	
}

void VSCSoundGeneratorGroup::removeGenerator(VSCSoundGeneratorPtr elem) {
	
	assert(elem);
	
	if (!elem)
		return;
	
	SynthSrcGenIter it = find (_generators.begin(), _generators.end(), elem);
	
	assert(it != _generators.end());
	
	if (it != _generators.end()) {
		_generators.erase(it);
	}
	
}

SynthSrcGenIter VSCSoundGeneratorGroup::beginGeneratorsIterator(void) {
	return _generators.begin();
}

SynthSrcGenIter VSCSoundGeneratorGroup::endGeneratorsIterator(void) {
	return _generators.end();
}

void VSCSoundGeneratorGroup::initialize(void) {
	
	VSCSoundGenerator::initialize();
	
	for (SynthSrcGenIter iter = _generators.begin(); iter != _generators.end(); iter++) {
        (*iter)->initialize();
    }
}

void VSCSoundGeneratorGroup::updateSoundEngine(void) {
	
	// call superclass implementation
	VSCSoundGenerator::updateSoundEngine();
	
	unsigned int numberOfChannels = this->getNumberOfChannels();
	
	/*
	 *	Synchronize the number of channels for all the generators, and update their soud engine 
	 */
	for (SynthSrcGenIter iter = _generators.begin(); iter != _generators.end(); iter++) {
        (*iter)->setNumberOfChannels(numberOfChannels);
		(*iter)->updateSoundEngine();
    }

}



