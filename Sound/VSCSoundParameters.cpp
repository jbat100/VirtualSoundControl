/*
 *  VSCSoundParameters.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 12/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundParameters.h"

#include <map>
#include <string>

typedef std::pair<VSCSParameter::Key,std::string> KeyLabelPair;
typedef std::pair<VSCSParameter::Key,std::pair<double, double> > KeyRangePair;
typedef std::pair<double, double> Range;

/*
 *	Static member variables initialization
 */

std::map<VSCSParameter::Key, std::string> VSCSParameter::parameterLabels;
bool VSCSParameter::generatedParameterLabels = false;
std::map<VSCSParameter::Key, std::pair<double, double> > VSCSParameter::parameterRanges;
bool VSCSParameter::generatedParameterRanges = false;

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


/*
 *	Multi-channel: channel index / key mapping
 */

int VSCSParameter::channelIndexForKey(VSCSParameter::Key k) {
	return (int)k - (int)KeyChannelZero;
}

VSCSParameter::Key VSCSParameter::keyForChannelIndex(int i) {
	
	if (i < 11) {
		Key k = (Key)(i + (int)KeyChannelZero);
		return k;
	}
	
    return VSCSParameter::KeyNone;
}

/*
 *	Parameter labels
 */

std::string VSCSParameter::labelForParameterWithKey(VSCSParameter::Key k) {
	
	if (generatedParameterLabels == false) 
		generateParameterLabels();
	
	std::map<Key,std::string>::iterator labelIterator = parameterLabels.find(k);
	
	if (labelIterator != parameterLabels.end()) {
		return labelIterator->second;
	}
	
	return "Error (Unknown Key)";
	
}

void VSCSParameter::generateParameterLabels(void) {
		
	parameterLabels.insert(KeyLabelPair (KeyNone, "None"));
	
	parameterLabels.insert(KeyLabelPair (KeyChannelZero,    "Channel 0"));
	parameterLabels.insert(KeyLabelPair (KeyChannelOne,     "Channel 1"));
	parameterLabels.insert(KeyLabelPair (KeyChannelTwo,     "Channel 2"));
	parameterLabels.insert(KeyLabelPair (KeyChannelThree,   "Channel 3"));
	parameterLabels.insert(KeyLabelPair (KeyChannelFour,    "Channel 4"));
	parameterLabels.insert(KeyLabelPair (KeyChannelFive,    "Channel 5"));
	parameterLabels.insert(KeyLabelPair (KeyChannelSix,     "Channel 6"));
	parameterLabels.insert(KeyLabelPair (KeyChannelSeven,   "Channel 7"));
	parameterLabels.insert(KeyLabelPair (KeyChannelEight,   "Channel 8"));
	parameterLabels.insert(KeyLabelPair (KeyChannelNine,    "Channel 9"));
	parameterLabels.insert(KeyLabelPair (KeyChannelTen,     "Channel 10"));
	
	parameterLabels.insert(KeyLabelPair (KeySineFrequency,  "Frequency (Hz)"));
	parameterLabels.insert(KeyLabelPair (KeySinePhase,      "Phase (degrees)"));
	
	parameterLabels.insert(KeyLabelPair (KeySquareFrequency,    "Frequency (Hz)"));
	parameterLabels.insert(KeyLabelPair (KeySquarePhase,        "Phase (degrees)"));
	parameterLabels.insert(KeyLabelPair (KeySquareHarmonics,    "Harmonics"));
	
	parameterLabels.insert(KeyLabelPair (KeySawFrequency,   "Frequency (Hz)"));
	parameterLabels.insert(KeyLabelPair (KeySawPhase,       "Phase (degrees)"));
	parameterLabels.insert(KeyLabelPair (KeySawHarmonics,   "Harmonics"));
	
	parameterLabels.insert(KeyLabelPair (KeyFileTime,                       "File Time (s)"));
	parameterLabels.insert(KeyLabelPair (KeyFilePlaybackSpeed,              "Playback Speed"));
	parameterLabels.insert(KeyLabelPair (KeyFileLoopOffset,                 "Loop Offset (s)"));
	parameterLabels.insert(KeyLabelPair (KeyFileLoopDuration,               "Loop Duration (s)"));
	parameterLabels.insert(KeyLabelPair (KeyFileLoopCrossoverDuration,      "Loop Crossover Duration (s)"));
	
	parameterLabels.insert(KeyLabelPair (KeyBiQuadType,         "Filter Type"));
	parameterLabels.insert(KeyLabelPair (KeyBiQuadFrequency,    "Frequency (Hz)"));
	parameterLabels.insert(KeyLabelPair (KeyBiQuadQFactor,      "Q Factor"));
	parameterLabels.insert(KeyLabelPair (KeyBiQuadLinearGain,   "Linear Gain"));
	
	generatedParameterLabels = true;
}

void VSCSParameter::generateParameterRanges(void) {
	
	parameterLabels.insert(KeyRangePair (KeyNone, Range(0.0, 0.0)));
	
	parameterLabels.insert(KeyRangePair (KeyChannelZero, defaultLinearAmplitudeRange));
	parameterLabels.insert(KeyRangePair (KeyChannelOne, defaultLinearAmplitudeRange));
	parameterLabels.insert(KeyRangePair (KeyChannelTwo, defaultLinearAmplitudeRange));
	parameterLabels.insert(KeyRangePair (KeyChannelThree, defaultLinearAmplitudeRange));
	parameterLabels.insert(KeyRangePair (KeyChannelFour, defaultLinearAmplitudeRange));
	parameterLabels.insert(KeyRangePair (KeyChannelFive, defaultLinearAmplitudeRange));
	parameterLabels.insert(KeyRangePair (KeyChannelSix, defaultLinearAmplitudeRange));
	parameterLabels.insert(KeyRangePair (KeyChannelSeven, defaultLinearAmplitudeRange));
	parameterLabels.insert(KeyRangePair (KeyChannelEight, defaultLinearAmplitudeRange));
	parameterLabels.insert(KeyRangePair (KeyChannelNine, defaultLinearAmplitudeRange));
	parameterLabels.insert(KeyRangePair (KeyChannelTen, defaultLinearAmplitudeRange));
	
	parameterLabels.insert(KeyRangePair (KeySineFrequency, defaultFrequencyRange ));
	parameterLabels.insert(KeyRangePair (KeySinePhase, defaultPhaseRange ));
	
	parameterLabels.insert(KeyRangePair (KeySquareFrequency, defaultFrequencyRange ));
	parameterLabels.insert(KeyRangePair (KeySquarePhase, defaultPhaseRange ));
	parameterLabels.insert(KeyRangePair (KeySquareHarmonics, defaultHarmonicsRange) );
	
	parameterLabels.insert(KeyRangePair (KeySawFrequency, defaultFrequencyRange));
	parameterLabels.insert(KeyRangePair (KeySawPhase, defaultPhaseRange));
	parameterLabels.insert(KeyRangePair (KeySawHarmonics, defaultHarmonicsRange));
	
	parameterLabels.insert(KeyRangePair (KeyFileTime, Range(0.0, 0.0)));
	parameterLabels.insert(KeyRangePair (KeyFilePlaybackSpeed, Range(0.0, 50.0)));
	parameterLabels.insert(KeyRangePair (KeyFileLoopOffset, Range(0.0, 0.0)));
	parameterLabels.insert(KeyRangePair (KeyFileLoopDuration, Range(0.0, 0.0)));
	parameterLabels.insert(KeyRangePair (KeyFileLoopCrossoverDuration, Range(0.0, 0.0)));
	
	parameterLabels.insert(KeyRangePair (KeyBiQuadFrequency, defaultFrequencyRange));
	parameterLabels.insert(KeyRangePair (KeyBiQuadQFactor, Range(0.0, 20.0)));
	parameterLabels.insert(KeyRangePair (KeyBiQuadLinearGain, defaultLinearFilterGainRange));
	
	generatedParameterRanges = true;
	
}

