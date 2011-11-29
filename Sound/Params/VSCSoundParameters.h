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
#include <boost/bimap.hpp>



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
	
	typedef std::pair<double, double>						ValueRange
	typedef std::pair<Key, std::string>						KeyLabelPair;
	typedef std::pair<Key, VSCSValueRange >					KeyRangePair;
	
	typedef std::set<Key>									KeySet;
	typedef std::map<Key, std::string>						KeyLabelMap;
	typedef std::map<Key, ValueRange >						KeyRangeMap;
	typedef std::map<Domain, KeySet >						DomainKeysMap;
	
	typedef boost::bimap<VSCSParameter::Key, int>			KeyIndexBimap;
	typedef KeyIndexBimap::value_type						KeyIndexBimapEntry;
	
	static const unsigned int kChannelNotFound;
	
	/*
	 *	Singleton instance
	 */
	static VSCSParameter& sharedInstance(void);

	
	/*
	 *	Constructor/Destructor
	 */
	VSCSParameter();
	~VSCSParameter();
	
	/*
	 *	Get all parameter keys related to a specific domain
	 */
	KeySet keysForDomain(Domain dom);
	
	/*
	 *	Channel volume/gain specific
	 */
	KeySet channelKeys(void);
	KeySet dBChannelKeys(void);
	unsigned int channelIndexForKey(Key k, bool* dB);
	Key keyForChannelIndex(unsigned int i, bool dB);
	bool parameterIsLinearChannel(Key k);
	bool parameterIsDBChannel(Key k);
	
	/*
	 *	Linear/dB amplitude/gain converions
	 */
	VSCSFloat linearToDBGain(VSCSFloat linearGain);
	VSCSFloat dBToLinearGain(VSCSFloat dBGain);
	
    /*
     *	Labels and ranges
     */
	std::string getLabelForParameterWithKey(Key k);
	void setLabelForParameterWithKey(std::string label, Key k);
	ValueRange getRangeForParameterWithKey(Key k);
	void setRangeForParameterWithKey(ValueRange valRange, Key k);
	
	/*
	 *	Default values
	 */
	ValueRange getDefaultLinearGainRange(void);
	
	/*
	 *	Fill VSCSParameterLabelMap and VSCSParameterRangeMap
	 */
	
	
private:
	
	KeyLabelMap parameterLabels;
	KeyRangeMap parameterRanges;
	
	void generateDefaultParameterLabels(void);
	void generateDefaultParameterRanges(void);
	KeyLabelMap defaultParameterLabels;
	KeyRangeMap defaultParameterRanges;
	
	void generateDomainParameters(void);
	DomainKeysMap domainParameters;
	
	
	unsigned int maxNumberOfChannels;
    
	
};



#endif
