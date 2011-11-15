/*
 *  VSCSoundParameters.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 12/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include <map>
#include <string>

/*-------------------------------------------------------------------------------------------
 *
 *	General sound parameter domain enum 
 */


/*-------------------------------------------------------------------------------------------
 *
 *	General sound parameter key enum 
 */


class VSCSParameter {
	
public:
	
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
	
	enum Key {
		
		KeyNone,
		
		// multi chan linear volume ...
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
	
	static int channelIndexForKey(Key k);
	static Key keyForChannelIndex(int i);
	
	static std::string labelForParameterWithKey(Key k);
	static std::pair<double, double> rangeForParameterWithKey(Key k);
	static std::pair<double, double> getDefaultLinearGainRange(void);
	
	
private:
	
	static std::map<Key, std::string> parameterLabels;
	static std::map<Key, std::pair<double, double> > parameterRanges;
	
	static std::pair<double, double> defaultLinearAmplitudeRange;
	static std::pair<double, double> defaultDBAmplitudeRange;
	static std::pair<double, double> defaultLinearFilterGainRange;
	static std::pair<double, double> defaultDBFilterGainRange;
	
	static bool generatedParameterLabels;
	static void generateParameterLabels(void);
	
	static bool generatedParameterRanges;
	static void generateParameterRanges(void);
	
};

