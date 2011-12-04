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
	
	static const unsigned int kChannelNotFound;
	static const unsigned int kIndexAll;
	
	/*-------------------------------------------------------------------------------------------
	 *
	 *	General sound parameter domain enum 
	 */
	
	enum Domain {
		
		DomainNone,
		
		DomainChannel,
		DomainSourceLocation,
		
		DomainSourceGenerator,
		DomainSourceGroup,
		DomainSourceFile,
		DomainSourceFileLoop,
		DomainSourceNoise,
		DomainSourceSaw,
		DomainSourceSine,
		DomainSourceSquare,
		
		DomainFilterBiQuad,
		
		DomainAll
	};
	
	/*-------------------------------------------------------------------------------------------
	 *
	 *	General sound parameter key enum 
	 */	
	
	enum Code {
		
		CodeNone,
		
		// channels 
		CodeGain,
		//CodeDBGain,
		
		// source location
		CodeDistance,
		CodeDegTheta,
		CodeDegPhi,
		CodePan, // special for 2 channel
		
		// sine, saw, square, pitched 
		CodeFrequency,
		//CodeLogFrequency,
		//CodeDegPhase,
		CodeRadPhase,
		CodeHarmonics,
		
		// filters
		CodeQFactor,
		
		// File and File loop
		CodeCurrentTime,
		CodePlaybackSpeed,
		CodeStartTime,
		CodeEndTime,
		CodeCrossoverDuration,
		
		CodeAll
		
	};
	
	struct Key {
		Domain domain;
		Code code;
		bool operator<(const Key& otherKey) const;

	};
	
    typedef std::set<Key>                                   KeySet;
    
	typedef std::pair<double, double>						ValueRange;
    
    typedef std::pair<Key, double>                          KeyValuePair;
	typedef std::pair<Key, std::string>						KeyLabelPair;
	typedef std::pair<Key, ValueRange >                     KeyRangePair;
									
    typedef std::map<Key, double>                           KeyValueMap;
	typedef std::map<Key, std::string>						KeyLabelMap;
	typedef std::map<Key, ValueRange >						KeyRangeMap;
	
    typedef std::map<Domain, KeySet >						DomainKeysMap;
	
	typedef boost::bimap<VSCSParameter::Key, int>			KeyIndexBimap;
	typedef KeyIndexBimap::value_type						KeyIndexBimapEntry;
	
	
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
	 *	Channel volume/gain specific
	 */
	KeySet singleChannelKeysUpToChannel(unsigned int chan);
	KeySet singleChannelDBKeysUpToChannel(unsigned int chan);
	
	/*
	 *	Linear/dB amplitude/gain converions
	 */
	VSCSFloat linearToDB(VSCSFloat linear);
	VSCSFloat dBToLinear(VSCSFloat dB);
	
    /*
     *	Labels and ranges (defaults if not set)
     */
	std::string getLabelForParameterWithKey(Key k);
	ValueRange getRangeForParameterWithKey(Key k);
	/*
	 *	Customize
	 */
	std::string setLabelForParameterWithKey(std::string label, Key k);
	ValueRange setRangeForParameterWithKey(ValueRange range, Key k);
	/*
	 *	Revert to defaults
	 */
	std::string revertLabelForParameterWithKeyToDefault(Key k);
	ValueRange revertRangeForParameterWithKeyToDefault(Key k);
	
	
	
private:
	
	KeyRangeMap customizedKeyRanges;
	KeyLabelMap customizedKeyLabels;
	
};



#endif
