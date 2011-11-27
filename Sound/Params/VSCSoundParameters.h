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
#include <set>



class VSCSParameter {
	
public:
	
	/*-------------------------------------------------------------------------------------------
	 *
	 *	General sound parameter domain enum 
	 */
	
	enum Domain {
		
		DomainNone,
		
		DomainChannels,
		
		DomainSourceGenerator,
		DomainSourceGroup,
		DomainSourceFile,
		DomainSourceFileLoop,
		DomainSourceNoise,
		DomainSourceSaw,
		DomainSourceSine,
		DomainSourceSquare,
		
		DomainFilterBiQuad
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
		
		// File loop
		KeyFileLoopTime,
		KeyFileLoopPlaybackSpeed,
		KeyFileLoopStartTime,
		KeyFileLoopEndTime,
		KeyFileLoopCrossoverDuration,
		
		// BiQuad Filter
		KeyBiQuadFrequency,
		KeyBiQuadQFactor,
		KeyBiQuadLinearGain,
		
		KeyAll
		
	};
	
	/*
	 *	Get all parameter keys related to a specific domain
	 */
	static std::set<Key> keysForDomain(Domain dom);
	
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
	 *	Linear/dB amplitude/gain converions
	 */
	static VSCSFloat linearToDBGain(VSCSFloat linearGain);
	static VSCSFloat dBToLinearGain(VSCSFloat dBGain);
	
    /*
     *	Labels and ranges
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
	static std::map<Domain, std::set<Key> > domainParameters;
	
	static std::pair<double, double> defaultLinearAmplitudeRange;
	static std::pair<double, double> defaultDBAmplitudeRange;
	static std::pair<double, double> defaultLinearFilterGainRange;
	static std::pair<double, double> defaultDBFilterGainRange;
    static std::pair<double, double> defaultPhaseRange;
    static std::pair<double, double> defaultFrequencyRange;
    static std::pair<double, double> defaultHarmonicsRange;
	
	static void generateParameterLabels(void);
	static void generateParameterRanges(void);
	static void generateDomainParameters(void);
    
	
};

typedef std::set<VSCSParameter::Key>								VSCSParameterKeySet;
typedef std::map<VSCSParameter::Key, std::string>					VSCSParameterLabelMap;
typedef std::pair<double, double>									VSCSValueRange
typedef std::map<VSCSParameter::Key, VSCSValueRange >				VSCSParameterRangeMap;
typedef std::pair<VSCSParameter::Key, std::string>					VSCSKeyLabelPair;
typedef std::pair<VSCSParameter::Key, VSCSValueRange >				VSCSKeyRangePair;


#endif
