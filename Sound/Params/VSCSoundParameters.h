/*
 *  VSCSoundParameters.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 12/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSCS_PARAMETER_H_
#define _VSCS_PARAMETER_H_

#include "VSCSound.h"
#include <map>
#include <string>



class VSCSParameter {
	
public:
	
	/*-------------------------------------------------------------------------------------------
	 *
	 *	General sound parameter domain enum 
	 */
	
	enum Domain {
		
		DomainNone,
		
		DomainSourceGenerator,
		DomainSourceGroup,
		DomainSourceFile,
		DomainSourceFileLoop,
		DomainSourceNoise,
		DomainSourceSaw,
		DomainSourceSine,
		DomainSourceSquare
	};
	
	/*-------------------------------------------------------------------------------------------
	 *
	 *	General sound parameter key enum 
	 */	
	
	enum Key {
		
		KeyNone,
		
		/*
		 *	multi chan linear volume ...
		 *	The order is IMPORTANT (used to calculate channel indeces)
		 */
		KeyChannelAll,
		KeyChannelZero,
		KeyChannelOne,
		KeyChannelTwo,
		KeyChannelThree,
		KeyChannelFour,
		KeyChannelFive,
		KeyChannelSix,
		KeyChannelSeven,
		KeyChannelEight,
		KeyChannelNine,
		KeyChannelTen,
		
		/*
		 *	multi chan dB volume ...
		 *	The order is IMPORTANT (used to calculate channel indeces)
		 */
		KeyChannelDBAll,
		KeyChannelDBZero,
		KeyChannelDBOne,
		KeyChannelDBTwo,
		KeyChannelDBThree,
		KeyChannelDBFour,
		KeyChannelDBFive,
		KeyChannelDBSix,
		KeyChannelDBSeven,
		KeyChannelDBEight,
		KeyChannelDBNine,
		KeyChannelDBTen,
		
		// Sine
		KeySineFrequency,
		KeySinePhase,
		
		// Square
		KeySquareFrequency,
		KeySquarePhase,
		KeySquareHarmonics,
		
		// Saw
		KeySawFrequency,
		KeySawPhase,
		KeySawHarmonics,
		
		// File 
		KeyFileTime,
		KeyFilePlaybackSpeed,
		KeyFileLoopOffset,
		KeyFileLoopDuration,
		KeyFileLoopCrossoverDuration,
		
		// BiQuad Filter
		KeyBiQuadType,
		KeyBiQuadFrequency,
		KeyBiQuadQFactor,
		KeyBiQuadLinearGain,
		
	};
	
	/*
	 *	Channel volume/gain specific
	 */
	static unsigned int channelIndexForKey(Key k, bool* dB);
	static Key keyForChannelIndex(unsigned int i, bool dB);
	static bool parameterIsLinearChannel(Key k);
	static bool parameterIsDBChannel(Key k);
    static const unsigned int kMaxNumberOfChannels;
    static const unsigned int kChannelNotFound;
	
    /*
     *
     */
	static std::string getLabelForParameterWithKey(Key k);
	static std::pair<double, double> getRangeForParameterWithKey(Key k);
	
	/*
	 *	Default values
	 */
	static std::pair<double, double> getDefaultLinearGainRange(void);
	
	
private:
	
	static std::map<Key, std::string> parameterLabels;
	static std::map<Key, std::pair<double, double> > parameterRanges;
	
	static std::pair<double, double> defaultLinearAmplitudeRange;
	static std::pair<double, double> defaultDBAmplitudeRange;
	static std::pair<double, double> defaultLinearFilterGainRange;
	static std::pair<double, double> defaultDBFilterGainRange;
    static std::pair<double, double> defaultPhaseRange;
    static std::pair<double, double> defaultFrequencyRange;
    static std::pair<double, double> defaultHarmonicsRange;
	
	static bool generatedParameterLabels;
	static void generateParameterLabels(void);
	
	static bool generatedParameterRanges;
	static void generateParameterRanges(void);
    
	
};

#endif
