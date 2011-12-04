/*
 *  VSCSoundGeneratorGroup.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 01/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_SYNTH_SOURCE_GROUP_
#define _VSC_SYNTH_SOURCE_GROUP_

#include "VSCSound.h"
#include "VSCSoundGenerator.h"

#include <list>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

#define VSCSoundGeneratorGroupPtr      boost::shared_ptr<VSCSoundGeneratorGroup>


#ifdef VSCS_USE_STK

#include "Stk.h"

class VSCSoundGeneratorGroup : public VSCSoundGenerator {
			
public:
	
	virtual void addGenerator(VSCSoundGeneratorPtr elem);
	virtual void removeGenerator(VSCSoundGeneratorPtr elem);
	
	SynthSrcGenIter beginGeneratorsIterator(void);
	SynthSrcGenIter endGeneratorsIterator(void);
    
    virtual void initialize(void);
    virtual void updateSoundEngine(void);
	
protected:
	
	void processComputationFrames(void);
	
	std::list<VSCSoundGeneratorPtr> _generators;
    
    stk::StkFrames _tempFrames;
	
};



inline void VSCSoundGeneratorGroup::processComputationFrames(void)
{  
    stk::zeroFrames(_computationFrames);
    
    if (_computationFrames.frames() != _tempFrames.frames() || _computationFrames.channels() != _tempFrames.channels()) 
        _tempFrames.resize(_computationFrames.frames(), _computationFrames.channels());
    
    for (SynthSrcGenIter iter = _generators.begin(); iter != _generators.end(); iter++) {
        (*iter)->tick(_tempFrames, kVSCSAllChannels);
        _computationFrames += _tempFrames;
    }

}

#endif // VSCS_USE_STK


#endif // _VSC_SYNTH_SOURCE_GROUP_