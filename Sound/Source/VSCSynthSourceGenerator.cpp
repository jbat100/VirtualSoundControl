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


void VSCSynthSourceGenerator::checkComputationFrames(stk::StkFrames& frames) {
	
}

void VSCSynthSourceGenerator::processComputationFrames(void) {
	
}

void VSCSynthSourceGroup::initialize(void) {
    this->updateSoundEngine();
}

void VSCSynthSourceGroup::updateSoundEngine(void) {

}
