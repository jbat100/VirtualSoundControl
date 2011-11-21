//
//  VSCSynthSourceGenerator.cpp
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/2/11.
//  Copyright (c) 2011 NXP. All rights reserved.
//

#include <iostream>
#include "VSCSynthSourceGenerator.h"

VSCSynthSourceGenerator::VSCSynthSourceGenerator() {
	group = NULL;
}

VSCSynthSourceGenerator::VSCSynthSourceGenerator(unsigned int numberOfChannels) {
	group = NULL;
}


VSCSFloat VSCSynthSourceGenerator::getValueForParameterWithKey(VSCSParameter::Key k) {
	VSCSynthSourceElement::getValueForParameterWithKey(k);
}

void VSCSynthSourceGenerator::setValueForParameterWithKey(double val, VSCSParameter::Key k) {
	VSCSynthSourceElement::setValueForParameterWithKey(val, k);
}


void VSCSynthSourceGenerator::checkComputationFrames(stk::StkFrames& frames) {
	
}

void VSCSynthSourceGenerator::processComputationFrames(void) {
	
}

void VSCSynthSourceGenerator::initialize(void) {
    this->updateSoundEngine();
}

void VSCSynthSourceGenerator::updateSoundEngine(void) {

}
