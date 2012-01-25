/*
 *  VSCFilter.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_FILTER_
#define _VSC_FILTER_

#include "VSCFrequencyPoint.h"
#include "VSCFilterUtils.h"

#include <boost/shared_ptr.hpp>

#include <vector>

#define ConstFreqPntIter			std::vector<VSCFrequencyPoint>::const_iterator 
#define ConstRevFreqPntIter			std::vector<VSCFrequencyPoint>::const_reverse_iterator 


class VSCFilter {
	
public:
	
	ConstFreqPntIter getFrequencyPointBeginConstIterator(void) const;
	ConstFreqPntIter getFrequencyPointEndConstIterator(void) const;
	
	void setBypass(bool b);
	bool isBypassed(void);
	
protected:
	
	bool _bypass;
	
	/*
	 *	there are arguments against using struct for frequency point and seperate into seperate vectors 
	 *	instead so as to make better use of the accelerate framework
	 */
	std::vector<VSCFrequencyPoint> _frequencyPoints;
	
	virtual void updateFrequencyPoints(void) = 0;
	
	VSCFilterUtilsPtr filterUtils;
	
};


#endif
