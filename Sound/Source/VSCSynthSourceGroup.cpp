/*
 *  VSCSynthSourceGroup.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 01/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSynthSourceGroup.h"

#include "Generator.h"
#include "VSCSTKUtils.h"
#include <algorithm>

void VSCSynthSourceGroup::addElement(VSCSynthSourceElementPtr elem) {
	
	assert(elem);
	
	if (!elem)
		return;
	
	ConstSynthSrcElemIter it = find (_elements.begin(), _elements.end(), elem);
	
	assert(it == _elements.end());
	
	if (it == _elements.end()) {
		_elements.push_back(elem);
		
		
		stk::Generator *gen = castVSCSynthSourceElementToStkGenerator(elem.get());
		
		assert(gen);
		
		if (gen) {
			_stkElements.push_back(gen);
		}
		
	}
	
}

void VSCSynthSourceGroup::removeElement(VSCSynthSourceElementPtr elem) {
	
	assert(elem);
	
	if (!elem)
		return;
	
	SynthSrcElemIter it = find (_elements.begin(), _elements.end(), elem);
	
	assert(it != _elements.end());
	
	if (it != _elements.end()) {
		_elements.erase(it);
	}
	
}

SynthSrcElemIter VSCSynthSourceGroup::beginElementsIterator(void) {
	return _elements.begin();
}

SynthSrcElemIter VSCSynthSourceGroup::endElementsIterator(void) {
	return _elements.end();
}

void VSCSynthSourceGroup::checkTempFramesForCompatibilityWith(stk::StkFrames& frames) {
	
}
