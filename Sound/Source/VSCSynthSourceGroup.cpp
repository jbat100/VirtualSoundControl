/*
 *  VSCSynthSourceGroup.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 01/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSynthSourceGroup.h"

#include "VSCSTKUtils.h"
#include <algorithm>

void VSCSynthSourceGroup::setNumberOfChannels(unsigned int numberOfChannels) {
    VSCSynthSourceGenerator::setNumberOfChannels(numberOfChannels);
    
    for (SynthSrcGenIter iter = _generators.begin(); iter != _generators.end(); iter++) {
        (*iter)->setNumberOfChannels(numberOfChannels);
    }
    
    _computationFrames.resize(_computationFrames.frames(), _numberOfChannels);
    _tempFrames.resize(_computationFrames.frames(), _numberOfChannels);
    
}

void VSCSynthSourceGroup::addGenerator(VSCSynthSourceGeneratorPtr elem) {
	
	assert(elem);
	
	if (!elem)
		return;
	
    assert(elem->getNumberOfChannels() == _numberOfChannels);
    
    if (elem->getNumberOfChannels() != _numberOfChannels) {
        assert(elem->numberOfChannelsIsLocked() == false);
        elem->setNumberOfChannels(_numberOfChannels);
    }
    
	ConstSynthSrcGenIter it = find (_generators.begin(), _generators.end(), elem);
	
	assert(it == _generators.end());
	
	if (it == _generators.end()) {
		_generators.push_back(elem);
	}
	
}

void VSCSynthSourceGroup::removeGenerator(VSCSynthSourceGeneratorPtr elem) {
	
	assert(elem);
	
	if (!elem)
		return;
	
	SynthSrcGenIter it = find (_generators.begin(), _generators.end(), elem);
	
	assert(it != _generators.end());
	
	if (it != _generators.end()) {
		_generators.erase(it);
	}
	
}

SynthSrcGenIter VSCSynthSourceGroup::beginGeneratorsIterator(void) {
	return _generators.begin();
}

SynthSrcGenIter VSCSynthSourceGroup::endGeneratorsIterator(void) {
	return _generators.end();
}

void VSCSynthSourceGroup::initialize(void) {
    this->updateSoundEngine();
}

void VSCSynthSourceGroup::updateSoundEngine(void) {
    if (_computationFrames.channels() != _numberOfChannels) {
        _computationFrames.resize(_computationFrames.frames(), _numberOfChannels);
    }
    if (_tempFrames.channels() != _numberOfChannels) {
        _tempFrames.resize(_tempFrames.frames(), _numberOfChannels);
    }
}



