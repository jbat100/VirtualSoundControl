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

typedef std::pair<VSCSParameter::Key,std::string> KeyLabelPair;
typedef std::pair<VSCSParameter::Key,std::pair<double, double> > KeyRangePair;
typedef std::pair<double, double> Range;

/*
 *	Static member variables initialization
 */

std::map<VSCSParameter::Key, std::string> VSCSParameter::parameterLabels;
std::map<VSCSParameter::Key, std::pair<double, double> > VSCSParameter::parameterRanges;
std::map<Domain, std::set<Key> > VSCSParameter::domainParameters;

/*
 *	Default ranges
 */
std::pair<double, double> VSCSParameter::defaultLinearAmplitudeRange(0.0, 1.0);
std::pair<double, double> VSCSParameter::defaultDBAmplitudeRange(-30.0, 0.0);
std::pair<double, double> VSCSParameter::defaultLinearFilterGainRange(0.0, 10.0);
std::pair<double, double> VSCSParameter::defaultDBFilterGainRange(-30.0, 10.0);
std::pair<double, double> VSCSParameter::defaultPhaseRange(-kVSC_PI, kVSC_PI);
std::pair<double, double> VSCSParameter::defaultFrequencyRange(20.0, 20000.0);
std::pair<double, double> VSCSParameter::defaultHarmonicsRange(0.0, 10.0);

const unsigned int VSCSParameter::kMaxNumberOfChannels = 11;


std::set<VSCSParameter::Key> VSCSParameter::keysForDomain(Domain dom) {
	return domainParameters[dom];
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
    
    //throw VSCSBadParameterException();
	
}

VSCSParameter::Key VSCSParameter::keyForChannelIndex(unsigned int i, bool dB) {
	
	if (i < VSCSParameter::kMaxNumberOfChannels) {
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
	VSCSFloat dB = 10.0*std::log10(linearGain)
    return  ;
}

VSCSFloat VSCSParameter::dBToLinearGain(VSCSFloat dBGain) {
    return 0.1*std::pow(10.0, dBGain);
}

/*
 *	Parameter labels
 */

std::string VSCSParameter::getLabelForParameterWithKey(VSCSParameter::Key k) {
	
	if (generatedParameterLabels == false) 
		VSCSParameter::generateParameterLabels();
	
	std::map<Key,std::string>::iterator labelIterator = parameterLabels.find(k);
	
	if (labelIterator != parameterLabels.end()) 
		return labelIterator->second;
	
    bool idDBChannel = false;
    int i = VSCSParameter::channelIndexForKey(k, &idDBChannel);
	if (idDBChannel) {
		return "Channel " + boost::lexical_cast<std::string>(i) + "Volume (Lin)";
	}
	else {
		return "Channel " + boost::lexical_cast<std::string>(i) + "Volume (dB)";
	}
	
	switch (k) {
		case KeySineFrequency:
		case KeySawFrequency:
		case KeySquareFrequency:
			return "Frequency (Hz)";
			break;
		case KeySinePhase:
		case KeySawPhase:
		case KeySquarePhase:
			return "Phase (rad)";
			break;
		case KeySawHarmonics:
		case KeySquareHarmonics:
			return "Harmonics";
			break;
		default:
			break;
	}
	
	return "Error (Unknown Key)";
	
}

std::pair<double, double> VSCSParameter::getRangeForParameterWithKey(Key k) {
	
	if (generatedParameterRanges == false) 
		VSCSParameter::generateParameterRanges();
	
	std::map<Key, std::pair<double, double> >::iterator rangeIterator = parameterRanges.find(k);
	
	if (rangeIterator != parameterRanges.end()) 
		return rangeIterator->second;
	
	
	if (VSCSParameter::parameterIsLinearChannel(k)) 
		return defaultLinearAmplitudeRange;
	
	if (VSCSParameter::parameterIsDBChannel(k))
		return defaultDBAmplitudeRange;
	
	switch (k) {
		case KeySineFrequency:
		case KeySawFrequency:
		case KeySquareFrequency:
			return defaultFrequencyRange;
			break;
		case KeySinePhase:
		case KeySawPhase:
		case KeySquarePhase:
			return defaultPhaseRange;
			break;
		case KeySawHarmonics:
		case KeySquareHarmonics:
			return defaultHarmonicsRange;
			break;
		default:
			break;
	}
	
	throw VSCSBadParameterException();
	
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

void VSCSParameter::generateParameterLabels(void) {
	
	static bool generatedParameterLabels = false;
	
	if (generatedParameterLabels == false) {
	
		parameterLabels.insert(KeyLabelPair (KeyFileTime, "File Time (s)"));
		parameterLabels.insert(KeyLabelPair (KeyFilePlaybackSpeed, "Playback Speed"));
		parameterLabels.insert(KeyLabelPair (KeyFileLoopOffset, "Loop Offset (s)"));
		parameterLabels.insert(KeyLabelPair (KeyFileLoopDuration, "Loop Duration (s)"));
		parameterLabels.insert(KeyLabelPair (KeyFileLoopCrossoverDuration, "Loop Crossover Duration (s)"));
		
		parameterLabels.insert(KeyLabelPair (KeyBiQuadFrequency, "Frequency (Hz)"));
		parameterLabels.insert(KeyLabelPair (KeyBiQuadQFactor, "Q Factor"));
		parameterLabels.insert(KeyLabelPair (KeyBiQuadLinearGain,   "Linear Gain"));
		
		generatedParameterLabels = true;
		
	}
}

void VSCSParameter::generateParameterRanges(void) {
	
	parameterRanges.insert(KeyRangePair (KeyNone, Range(0.0, 0.0)));
	
	parameterRanges.insert(KeyRangePair (KeyFileTime, Range(0.0, 0.0)));
	parameterRanges.insert(KeyRangePair (KeyFilePlaybackSpeed, Range(0.0, 50.0)));
	parameterRanges.insert(KeyRangePair (KeyFileLoopOffset, Range(0.0, 0.0)));
	parameterRanges.insert(KeyRangePair (KeyFileLoopDuration, Range(0.0, 0.0)));
	parameterRanges.insert(KeyRangePair (KeyFileLoopCrossoverDuration, Range(0.0, 0.0)));
	
	parameterRanges.insert(KeyRangePair (KeyBiQuadFrequency, defaultFrequencyRange));
	parameterRanges.insert(KeyRangePair (KeyBiQuadQFactor, Range(0.0, 20.0)));
	parameterRanges.insert(KeyRangePair (KeyBiQuadLinearGain, defaultLinearFilterGainRange));
	
	generatedParameterRanges = true;
	
}

void VSCSParameter::generateDomainParameters(void) {
	
	/*
	 *	Make sure this is run only once
	 */
	static bool generatesDomainParameters = false;
	
	if (generatesDomainParameters == false) {
	
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

		
		generatesDomainParameters = true;

	}
	
	
}

