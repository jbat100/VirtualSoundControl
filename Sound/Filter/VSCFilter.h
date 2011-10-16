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

#include <list>

#define ConstFreqPntIter			std::list<VSCFrequencyPoint>::const_iterator 
#define ConstRevFreqPntIter			std::list<VSCFrequencyPoint>::const_reverse_iterator 

typedef struct VSCFrequencyPoint {
	double frequency;
	double magnitude;
	double phase;
} VSCFrequencyPoint;

class VSCFilter {
	
public:
	
	virtual void updateFrequencyPoints(void) = 0;
	
	ConstFreqPntIter getFrequencyPointBeginConstIterator(void) const;
	ConstFreqPntIter getFrequencyPointEndConstIterator(void) const;
	
	void setBypass(bool b);
	bool isBypassed(void);
	
protected:
	
	bool _bypass;
	std::list<VSCFrequencyPoint> _frequencyPoints;
	
};


#endif
