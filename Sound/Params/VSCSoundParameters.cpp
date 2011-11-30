/*
 *  VSCSoundParameters.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 12/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundParameters.h"
#include "VSCException.h"

#include <map>
#include <string>
#include <boost/lexical_cast.hpp>

const unsigned int VSCSParameter::kChannelNotFound = kVSCSVoidChannel;


VSCSParameter::VSCSParameter() {
	
	kMaxNumberOfChannels = 11;
	
	this->generateDomainParameters();
								
	this->generateDefaultParameterLabels();
	this->generateDefaultParameterRanges();
	
}

VSCSParameter::~VSCSParameter() {
	
}

VSCSParameter& VSCSParameter::sharedInstance(void) {
	static VSCSParameter singletonInstance;
	return singletonInstance;
}


VSCSParameter::KeySet VSCSParameter::keysForDomain(Domain dom) {
	return domainParameters[dom];
}

VSCSParameter::KeySet VSCSParameter::channelKeys(void) {
	KeySet keys;
	keys.insert(KeyChannelAll);
	for (unsigned int i = 0; i < maxNumberOfChannels; i++) {
		Key k = this->keyForChannelIndex(i, false);
		keys.insert(k);
	}
	return keys;
}

VSCSParameter::KeySet VSCSParameter::dBChannelKeys(void) {
	KeySet keys;
	keys.insert(KeyChannelDBAll);
	for (unsigned int i = 0; i < maxNumberOfChannels; i++) {
		Key k = this->keyForChannelIndex(i, true);
		keys.insert(k);
	}
	return keys;
}

/*
 *	Multi-channel: channel index / key mapping
 */

unsigned int VSCSParameter::channelIndexForKey(VSCSParameter::Key k, bool* dB) {
    
    if (k == KeyChannelAll) {
        if (dB != NULL) *dB = false;
        return kVSCSAllChannels;
    }
	
    else if (k == KeyChannelDBAll) {
        if (dB != NULL) *dB = true;
        return kVSCSAllChannels;
    }
    
	else if (VSCSParameter::parameterIsLinearChannel(k)) {
        if (dB != NULL) *dB = false;
		return (int)k - (int)KeyChannelZero;
	}
	else if (VSCSParameter::parameterIsDBChannel(k)) {
        if (dB != NULL) *dB = true;
		return (int)k - (int)KeyChannelDBZero;
	}
    
    return kVSCSVoidChannel;
	
}

VSCSParameter::Key VSCSParameter::keyForChannelIndex(unsigned int i, bool dB) {
	
	if (i < maxNumberOfChannels) {
        if (dB) {
            Key k = (Key)(i + (int)KeyChannelDBZero);
            return k;
        }
        else {
            Key k = (Key)(i + (int)KeyChannelZero);
            return k;
        }
	}
	
    throw VSCSBadParameterException();
}

/*
 *	This should convert so that linear volume gain [0 : 1] becomes [-infinity : 0]
 */
VSCSFloat VSCSParameter::linearToDBGain(VSCSFloat linearGain) {
	VSCSFloat dB = 10.0*std::log10(linearGain);
    return  dB;
}

VSCSFloat VSCSParameter::dBToLinearGain(VSCSFloat dBGain) {
    return 0.1*std::pow(10.0, dBGain);
}

/*
 *	Parameter labels
 */

std::string VSCSParameter::getLabelForParameterWithKey(VSCSParameter::Key k) {
	
	KeyLabelMap::iterator labelIterator = parameterLabels.find(k);
	
	if (labelIterator != parameterLabels.end()) 
		return labelIterator->second;
	
	throw VSCSBadParameterException();
	
}

void VSCSParameter::setLabelForParameterWithKey(std::string label, Key k) {
	
	KeyLabelMap::iterator labelIterator = parameterLabels.find(k);
	
	if (labelIterator != parameterLabels.end()) 
		parameterLabels.erase(k);
	
	parameterLabels.insert(KeyLabelPair (k,label));
	
}

VSCSParameter::ValueRange VSCSParameter::getRangeForParameterWithKey(Key k) {
	
	KeyRangeMap::iterator rangeIterator = parameterRanges.find(k);
	
	if (rangeIterator != parameterRanges.end()) 
		return rangeIterator->second;
	
	throw VSCSBadParameterException();
	
}

void VSCSParameter::setRangeForParameterWithKey(ValueRange valRange, Key k) {
	
	KeyRangeMap::iterator rangeIterator = parameterRanges.find(k);
	
	if (rangeIterator != parameterRanges.end()) 
		parameterRanges.erase(k);
	
	parameterRanges.insert(KeyRangePair (k, valRange));
	
}


bool VSCSParameter::parameterIsLinearChannel(Key k) {
	
	switch (k) {
		case KeyChannelAll:
		case KeyChannelZero:
		case KeyChannelOne: 
		case KeyChannelTwo:  
		case KeyChannelThree:
		case KeyChannelFour: 
		case KeyChannelFive: 
		case KeyChannelSix:  
		case KeyChannelSeven:
		case KeyChannelEight:
		case KeyChannelNine: 
		case KeyChannelTen:
			return true;
			break;
		default:
			break;
	}
	
	return false;
	
}

bool VSCSParameter::parameterIsDBChannel(Key k) {
	
	switch (k) {
		case KeyChannelDBAll:
		case KeyChannelDBZero:
		case KeyChannelDBOne: 
		case KeyChannelDBTwo:  
		case KeyChannelDBThree:
		case KeyChannelDBFour: 
		case KeyChannelDBFive: 
		case KeyChannelDBSix:  
		case KeyChannelDBSeven:
		case KeyChannelDBEight:
		case KeyChannelDBNine: 
		case KeyChannelDBTen:
			return true;
			break;
		default:
			break;
	}
	
	return false;
	
}

void VSCSParameter::generateDefaultParameterLabels(void) {
	
	std::string frequencyLabel = "Frequency (Hz)";
	std::string phaseLabel = "Phase (rad)";
	std::string harmonicsLabel = "Harmonics";
	
	parameterLabels.insert(KeyLabelPair (KeyChannelAll, "Volume (Lin)"));
	parameterLabels.insert(KeyLabelPair (KeyChannelDBAll, "Volume (dB)"));
	for (unsigned int i = 0; i < maxNumberOfChannels; i++) {
		Key k = keyForChannelIndex(i, false);
		std::string label = "Channel " + boost::lexical_cast<std::string>(i) + "Volume (Lin)";
		parameterLabels.insert(KeyLabelPair (k, label));
		Key dBK = keyForChannelIndex(i, true);
		std::string dBLabel = "Channel " + boost::lexical_cast<std::string>(i) + "Volume (dB)";
		parameterLabels.insert(KeyLabelPair (dBK, dBLabel));
	}
	
	parameterLabels.insert(KeyLabelPair (KeySineFrequency, frequencyLabel));
	parameterLabels.insert(KeyLabelPair (KeySawFrequency, frequencyLabel));
	parameterLabels.insert(KeyLabelPair (KeySquareFrequency, frequencyLabel));
	
	parameterLabels.insert(KeyLabelPair (KeySinePhase, phaseLabel));
	parameterLabels.insert(KeyLabelPair (KeySawPhase, phaseLabel));
	parameterLabels.insert(KeyLabelPair (KeySquarePhase, phaseLabel));
	
	parameterLabels.insert(KeyLabelPair (KeySawHarmonics, harmonicsLabel));
	parameterLabels.insert(KeyLabelPair (KeySquareHarmonics, harmonicsLabel));
	
	parameterLabels.insert(KeyLabelPair (KeyFileTime, "File Time (s)"));
	parameterLabels.insert(KeyLabelPair (KeyFilePlaybackSpeed, "Playback Speed"));
	parameterLabels.insert(KeyLabelPair (KeyFileLoopStartTime, "Loop start time (s)"));
	parameterLabels.insert(KeyLabelPair (KeyFileLoopEndTime, "Loop end time (s)"));
	parameterLabels.insert(KeyLabelPair (KeyFileLoopCrossoverDuration, "Loop Crossover Duration (s)"));
	
	parameterLabels.insert(KeyLabelPair (KeyBiQuadFrequency, frequencyLabel));
	parameterLabels.insert(KeyLabelPair (KeyBiQuadQFactor, "Q Factor"));
	parameterLabels.insert(KeyLabelPair (KeyBiQuadLinearGain,   "Linear Gain"));

}

void VSCSParameter::generateDefaultParameterRanges(void) {
	
	ValueRange defaultLinearAmplitudeRange (0.0, 1.0);
	ValueRange defaultDBAmplitudeRange (-30.0, 0.0);
	ValueRange defaultLinearFilterGainRange (0.0, 10.0);
	ValueRange defaultDBFilterGainRange (-30.0, 10.0);
    ValueRange defaultPhaseRange = (-kVSC_PI, kVSC_PI);
    ValueRange defaultFrequencyRange (20.0, 20000.0);
    ValueRange defaultHarmonicsRange (0.0, 10.0);
	
	parameterRanges.insert(KeyRangePair (KeyNone, ValueRange(0.0, 0.0)));
	
	parameterRanges.insert(KeyLabelPair (KeyChannelAll, defaultLinearAmplitudeRange));
	parameterRanges.insert(KeyLabelPair (KeyChannelDBAll, defaultDBAmplitudeRange));
	for (unsigned int i = 0; i < maxNumberOfChannels; i++) {
		Key k = keyForChannelIndex(i, false);
		parameterLabels.insert(KeyLabelPair (k, defaultLinearAmplitudeRange));
		Key dBK = keyForChannelIndex(i, true);
		parameterLabels.insert(KeyLabelPair (dBK, defaultDBAmplitudeRange));
	}

	parameterRanges.insert(KeyRangePair (KeySineFrequency, defaultFrequencyRange));
	parameterRanges.insert(KeyRangePair (KeySinePhase, defaultPhaseRange));
	
	parameterRanges.insert(KeyRangePair (KeySawFrequency, defaultFrequencyRange));
	parameterRanges.insert(KeyRangePair (KeySawPhase, defaultPhaseRange));
	parameterRanges.insert(KeyRangePair (KeySawHarmonics, defaultHarmonicsRange));
	
	parameterRanges.insert(KeyRangePair (KeySquareFrequency, defaultFrequencyRange));
	parameterRanges.insert(KeyRangePair (KeySquarePhase, defaultPhaseRange));
	parameterRanges.insert(KeyRangePair (KeySquareHarmonics, defaultHarmonicsRange));
	
	
	parameterRanges.insert(KeyRangePair (KeyFileTime, ValueRange(0.0, 0.0)));
	parameterRanges.insert(KeyRangePair (KeyFilePlaybackSpeed, ValueRange(0.0, 50.0)));
	parameterRanges.insert(KeyRangePair (KeyFileLoopStartTime, ValueRange(0.0, 0.0)));
	parameterRanges.insert(KeyRangePair (KeyFileLoopEndTime, ValueRange(0.0, 0.0)));
	parameterRanges.insert(KeyRangePair (KeyFileLoopCrossoverDuration, ValueRange(0.0, 0.0)));
	
	parameterRanges.insert(KeyRangePair (KeyBiQuadFrequency, defaultFrequencyRange));
	parameterRanges.insert(KeyRangePair (KeyBiQuadQFactor, ValueRange(0.0, 20.0)));
	parameterRanges.insert(KeyRangePair (KeyBiQuadLinearGain, defaultLinearFilterGainRange));
	
}

void VSCSParameter::generateDomainParameters(void) {
	
	/*
	 *	Channel related keys
	 */
	std::set<Key> channelKeys;
	channelKeys.insert(KeyChannelAll);
	channelKeys.insert(KeyChannelZero);
	channelKeys.insert(KeyChannelOne);
	channelKeys.insert(KeyChannelTwo);
	channelKeys.insert(KeyChannelThree);
	channelKeys.insert(KeyChannelFour);
	channelKeys.insert(KeyChannelFive);
	channelKeys.insert(KeyChannelSix);
	channelKeys.insert(KeyChannelSeven);
	channelKeys.insert(KeyChannelEight);
	channelKeys.insert(KeyChannelNine);
	channelKeys.insert(KeyChannelTen);
	channelKeys.insert(KeyChannelDBAll);
	channelKeys.insert(KeyChannelDBZero);
	channelKeys.insert(KeyChannelDBOne);
	channelKeys.insert(KeyChannelDBTwo);
	channelKeys.insert(KeyChannelDBThree);
	channelKeys.insert(KeyChannelDBFour);
	channelKeys.insert(KeyChannelDBFive);
	channelKeys.insert(KeyChannelDBSix);
	channelKeys.insert(KeyChannelDBSeven);
	channelKeys.insert(KeyChannelDBEight);
	channelKeys.insert(KeyChannelDBNine);
	channelKeys.insert(KeyChannelDBTen);
	domainParameters[DomainChannels] = channelKeys;

	std::set<Key> sourceFileKeys;
	sourceFileKeys.insert(KeyFileTime);
	sourceFileKeys.insert(KeyFilePlaybackSpeed);
	domainParameters[DomainSourceFile] = sourceFileKeys;
	
	std::set<Key> sourceFileLoopKeys;
	sourceFileLoopKeys.insert(KeyFileLoopTime);
	sourceFileLoopKeys.insert(KeyFileLoopPlaybackSpeed);
	sourceFileLoopKeys.insert(KeyFileLoopStartTime);
	sourceFileLoopKeys.insert(KeyFileLoopEndTime);
	sourceFileLoopKeys.insert(KeyFileLoopCrossoverDuration);
	domainParameters[DomainSourceFileLoop] = sourceFileLoopKeys;
	
	std::set<Key> sourceSawKeys;
	sourceSawKeys.insert(KeySawFrequency);
	sourceSawKeys.insert(KeySawPhase);
	sourceSawKeys.insert(KeySawHarmonics);
	domainParameters[DomainSourceSaw] = sourceSawKeys;
	
	std::set<Key> sourceSquareKeys;
	sourceSquareKeys.insert(KeySquareFrequency);
	sourceSquareKeys.insert(KeySquarePhase);
	sourceSquareKeys.insert(KeySquareHarmonics);
	domainParameters[DomainSourceSquare] = sourceSquareKeys;
	
	std::set<Key> sourceSineKeys;
	sourceSineKeys.insert(KeySawFrequency);
	sourceSineKeys.insert(KeySawPhase);
	domainParameters[DomainSourceSine] = sourceSineKeys;
	
	std::set<Key> filterBiquadKeys;
	filterBiquadKeys.insert(KeyBiQuadFrequency);
	filterBiquadKeys.insert(KeyBiQuadQFactor);
	filterBiquadKeys.insert(KeyBiQuadLinearGain);
	domainParameters[DomainFilterBiQuad] = filterBiquadKeys;
	
}

