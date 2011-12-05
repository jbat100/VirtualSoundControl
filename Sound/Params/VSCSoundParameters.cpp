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
#include "VSCMath.h"

#include <map>
#include <string>
#include <boost/lexical_cast.hpp>

const unsigned int VSCSParameter::kChannelNotFound = kVSCSVoidChannel;
const unsigned int VSCSParameter::kIndexAll = UINT_MAX;


bool VSCSParameter::Key::operator<(const Key& otherKey) const {
	if (domain < otherKey.domain)
		return true;
	if (code < otherKey.code)
		return true;
	return false;
}

VSCSParameter::VSCSParameter() {
	
}

VSCSParameter::~VSCSParameter() {
	
}

VSCSParameter& VSCSParameter::sharedInstance(void) {
	static VSCSParameter singletonInstance;
	return singletonInstance;
}

/*
 *	This should convert so that linear volume gain [0 : 1] becomes [-infinity : 0]
 */
VSCSFloat VSCSParameter::linearToDB(VSCSFloat linear) {
	VSCSFloat dB = 10.0*std::log10(linear);
    return  dB;
}

VSCSFloat VSCSParameter::dBToLinear(VSCSFloat dB) {
    return 0.1*std::pow(10.0, dB);
}


/*
 *	Parameter labels
 */
std::string VSCSParameter::getLabelForParameterWithKey(VSCSParameter::Key k) {
	
	/*
	 *	Check if there are exact matches
	 */
	KeyLabelMap::iterator labelIterator = customizedKeyLabels.find(k);
	if (labelIterator != customizedKeyLabels.end()) 
		return labelIterator->second;
	
	switch (k.code) {
		case CodeGain:
			return "Gain (Lin)";
//		case CodeDBGain:
//			return "Gain (dB)";
		case CodeFrequency:
			return "Freq (Hz)";
//		case CodeLogFrequency:
//			return "Freq (Log Hz)";
//		case CodeDegPhase:
//			return "Phase (deg)";
		case CodeRadPhase:
			return "Phase (rad)";
		case CodeHarmonics:
			return "Harmonics";
		default:
			return "";
			break;
	}
	
	throw VSCSBadParameterException();
	
}

void VSCSParameter::setLabelForParameterWithKey(std::string label, Key k) {
	
	KeyLabelMap::iterator labelIterator = customizedKeyLabels.find(k);
	if (labelIterator != customizedKeyLabels.end()) 
		customizedKeyLabels.erase(k);
	
	customizedKeyLabels.insert(KeyLabelPair (k, label));
	
}

std::string VSCSParameter::revertLabelForParameterWithKeyToDefault(Key k) {
	
	KeyLabelMap::iterator labelIterator = customizedKeyLabels.find(k);
	if (labelIterator != customizedKeyLabels.end()) 
		customizedKeyLabels.erase(k);
	
}


VSCSParameter::ValueRange VSCSParameter::getRangeForParameterWithKey(Key k) {
	
	KeyRangeMap::iterator rangeIterator = customizedKeyRanges.find(k);
	if (rangeIterator != customizedKeyRanges.end()) 
		return rangeIterator->second;
	
	switch (k.code) {
		case CodeGain:
			return ValueRange (0.0, 1.0);
//		case CodeDBGain:
//			return ValueRange (-30.0, 0.0);
		case CodeFrequency:
			return ValueRange (20.0, 20000.0);
//		case CodeLogFrequency:
//			return ValueRange (std::log10(20.0), std::log10(20000.0));
//		case CodeDegPhase:
//			return ValueRange (-180.0, 180.0);
		case CodeRadPhase:
			return ValueRange (-vsc::kPI, vsc::kPI);
		case CodeHarmonics:
			return ValueRange (0.0, 10.0);
		default:
			return ValueRange (0.0, 0.0);
			break;
	}
	
	throw VSCSBadParameterException();
	
}

void VSCSParameter::setRangeForParameterWithKey(ValueRange valRange, Key k) {
	
	KeyRangeMap::iterator rangeIterator = customizedKeyRanges.find(k);
	if (rangeIterator != customizedKeyRanges.end()) 
		customizedKeyRanges.erase(k);
	
	customizedKeyRanges.insert(KeyRangePair (k, valRange));
	
}

VSCSParameter::ValueRange VSCSParameter::revertRangeForParameterWithKeyToDefault(Key k) {
	
	KeyRangeMap::iterator rangeIterator = customizedKeyRanges.find(k);
	if (rangeIterator != customizedKeyRanges.end()) 
		customizedKeyRanges.erase(k);
	
}

