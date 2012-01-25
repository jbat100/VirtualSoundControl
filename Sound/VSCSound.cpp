/*
 *  VSCSound.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 22/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSound.h"

#include <cmath>
#include <cassert>

/*
 *	Static member variable initialization
 */

std::vector<VSCSFloat> VSCSound::midiNoteFrequencies;
VSCSFloat VSCSound::referenceAFrequency = 440.0;
bool VSCSound::midiNoteFrequenciesAreComputed = false;


VSCSFloat VSCSound::logFrequencyToFrequency(VSCSFloat logFreq) {
	return std::pow(10.0, logFreq);
}

VSCSFloat VSCSound::frequencyToLogFrequency(VSCSFloat freq) {
	return std::log10(freq);
}

VSCSFloat VSCSound::frequencyForMidiNote(VSCSFloat midiNote) {
	return VSCSound::frequencyForMidiNote((unsigned int)midiNote);
}

VSCSFloat VSCSound::frequencyForMidiNote(unsigned int midiNote) {
	assert(midiNote >= 0 && midiNote < 127);
	if (VSCSound::midiNoteFrequenciesAreComputed == false)
		VSCSound::computeMidiNoteFrequencies();
	return VSCSound::midiNoteFrequencies[midiNote];
}

void VSCSound::setReferenceAFrequency(VSCSFloat f) {
	VSCSound::referenceAFrequency = f;
}

VSCSFloat VSCSound::getReferenceAFrequency(void) {
	return VSCSound::referenceAFrequency;
}

void VSCSound::computeMidiNoteFrequencies(void) {
	VSCSound::midiNoteFrequencies.resize(128);
	for (int x = 0; x < 127; ++x) {
		VSCSFloat freq = (VSCSound::referenceAFrequency / 32.0) * (std::pow(2.0, ((x - 9.0) / 12.0)));
		VSCSound::midiNoteFrequencies[x] = freq;
	}
	VSCSound::midiNoteFrequenciesAreComputed = true;
}
