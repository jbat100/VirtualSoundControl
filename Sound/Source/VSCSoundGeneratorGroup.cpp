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
    
	ConstSynthSrcGenIter it = find (_generators.begin(), _generators.end(), elem);
	
	if (it == _generators.end()) {
		_generators.push_back(elem);
	}
	
}

void VSCSoundGeneratorGroup::removeGenerator(VSCSoundGeneratorPtr elem) {
	
	assert(elem);
	
	if (!elem)
		return;
	
	SynthSrcGenIter it = find (_generators.begin(), _generators.end(), elem);
	
	if (it != _generators.end()) {
		_generators.erase(it);
	}
	
}

const std::list<VSCSoundGeneratorPtr>& VSCSoundGeneratorGroup::generators(void) {
	return _generators;
}

void VSCSoundGeneratorGroup::initialize(void) {
	VSCSoundGenerator::initialize();
	for (SynthSrcGenIter iter = _generators.begin(); iter != _generators.end(); iter++) {
        (*iter)->initialize();
    }
}




