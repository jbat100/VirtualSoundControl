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
#include "VSCSynthSourceGenerator.h"

#include <list>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>


#ifdef VSCS_USE_STK

#include "Stk.h"

class VSCSynthSourceGroup : public VSCSynthSourceGenerator {
			
public:
	
	virtual void addGenerator(VSCSynthSourceGeneratorPtr elem);
	virtual void removeGenerator(VSCSynthSourceGeneratorPtr elem);
	
	SynthSrcGenIter beginGeneratorsIterator(void);
	SynthSrcGenIter endGeneratorsIterator(void);
	
protected:
	
	void processComputationFrames(void);
	
	std::list<VSCSynthSourceGeneratorPtr> _generators;
	
	/*
     *  keep an StkFrames so that it does not need to be created everytime the tick function is called
	 *  this is used to add the frames of all the generators 
     */
	stk::StkFrames _tempFrames;
    /*
     *  Check that _tempFrames has the same number of frames and channels as argument
     */
	virtual void checkTempFrames(stk::StkFrames& frames);
	
};



inline void VSCSynthSourceGroup::processComputationFrames(void)
{

}

#endif // VSCS_USE_STK


#endif // _VSC_SYNTH_SOURCE_GROUP_