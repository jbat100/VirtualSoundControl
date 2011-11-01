/*
 *  VSCSynthSourceGroup.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 01/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_SYNTH_SOURCE_GROUP_
#define _VSC_SYNTH_SOURCE_GROUP_

#include "VSCSound.h"
#include "VSCSynthSourceElement.h"

#include <list>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>


#ifdef VSCS_USE_STK

#include "Stk.h"
#include "Generator.h"

class VSCSynthSourceGroup : public VSCSynthSourceElement, public stk::Generator {

//class VSCSynthSourceGroup : public stk::Generator {
			
public:
	
	void addElement(VSCSynthSourceElementPtr elem);
	void removeElement(VSCSynthSourceElementPtr elem);
	
	SynthSrcElemIter beginElementsIterator(void);
	SynthSrcElemIter endElementsIterator(void);
	
	virtual stk::StkFrames& tick(stk::StkFrames& frames, unsigned int channel = 0);
	
protected:
	
	std::list<VSCSynthSourceElementPtr> _elements;
	

	// keep a list of elements (same as _elements) casted to stk::Generator for faster tick computes
	std::list<stk::Generator*> _stkElements;
	// keep an StkFrames so that it does not need to be created everytime the tick function is called
	stk::StkFrames _tempFrames;
	void checkTempFramesForCompatibilityWith(stk::StkFrames& frames);
	
};



inline stk::StkFrames& VSCSynthSourceGroup::tick(stk::StkFrames& frames, unsigned int channel)
{

	return frames;
}

#endif // VSCS_USE_STK


#endif // _VSC_SYNTH_SOURCE_GROUP_