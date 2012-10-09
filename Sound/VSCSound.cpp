/*
 *  VSCSound.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 22/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSound.h"

#include <boost/assert.hpp>

#include <cmath>
#include <cassert>

VSC::Sound::Pitch::Pitch() : mReferenceAFrequency(440.0)
{
    computeMidiNoteFrequencies();
}

VSC::Sound::Float VSC::Sound::Pitch::logFrequencyToFrequency(Float logFreq)
{
	return std::pow(10.0, logFreq);
}

VSC::Sound::Float VSC::Sound::Pitch::frequencyToLogFrequency(Float freq)
{
	return std::log10(freq);
}

VSC::Sound::Float VSC::Sound::Pitch::frequencyForMidiNote(Float midiNote)
{
	return frequencyForMidiNote((unsigned int)midiNote);
}

VSC::Sound::Float VSC::Sound::Pitch::frequencyForMidiNote(unsigned int midiNote)
{
	BOOST_ASSERT_MSG(midiNote >= 0 && midiNote < 127, "MIDI note should be in range [0-127]");
	return mMIDINoteFrequencies[midiNote];
}

void VSC::Sound::Pitch::setReferenceAFrequency(Float f)
{
	mReferenceAFrequency = f;
    computeMidiNoteFrequencies();
}

VSC::Sound::Float VSC::Sound::Pitch::getReferenceAFrequency(void)
{
	return mReferenceAFrequency;
}

void VSC::Sound::Pitch::computeMidiNoteFrequencies(void)
{
	mMIDINoteFrequencies.resize(128);
	for (int x = 0; x < 127; ++x)
    {
		Float freq = (mReferenceAFrequency / 32.0) * (std::pow(2.0, ((x - 9.0) / 12.0)));
		mMIDINoteFrequencies[x] = freq;
	}
}
