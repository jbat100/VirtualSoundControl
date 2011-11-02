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

void VSCSynthSourceGroup::addGenerator(VSCSynthSourceGeneratorPtr elem) {
	
	assert(elem);
	
	if (!elem)
		return;
	
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

void VSCSynthSourceGroup::checkTempFrames(stk::StkFrames& frames) {
	if (_tempFrames.channels() == frames.channels() || _tempFrames.frames() == frames.frames())
        _tempFrames.resize(frames.frames(), frames.channels());
}
