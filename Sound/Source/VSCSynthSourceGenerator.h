//
//  VSCSynthSourceGenerator.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/2/11.
//  Copyright (c) 2011 NXP. All rights reserved.
//

#ifndef _VSC_SYNTH_SOURCE_GENERATOR_
#define _VSC_SYNTH_SOURCE_GENERATOR_


#ifdef VSCS_USE_STK

#include "VSCSynthSourceElement.h"

#include "Stk.h"
#include "Generator.h"

#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>

#define VSCSynthSourceGeneratorPtr      boost::shared_ptr<VSCSynthSourceGenerator>

#define ConstSynthSrcGenIter            std::list<VSCSynthSourceGeneratorPtr>::const_iterator 
#define ConstRevSynthSrcGenIter         std::list<VSCSynthSourceGeneratorPtr>::const_reverse_iterator 

#define SynthSrcGenIter                 std::list<VSCSynthSourceGeneratorPtr>::iterator 
#define RevSynthSrcGenIter              std::list<VSCSynthSourceGeneratorPtr>::reverse_iterator 

class VSCSynthSourceGenerator : public VSCSynthSourceElement, public stk::Generator {
    
    //class VSCSynthSourceGroup : public stk::Generator {
    
public:
	
    /*
     *  stk::Generator tick method which should be over-ridden.
     */
	stk::StkFrames& tick(stk::StkFrames& frames, unsigned int channel = 0);
    stk::StkFloat tick(void);
	
protected:
	
	/* 
     *  keep a seperate StkFrames so that it does not need to be created everytime the tick function is called
     *  this is used by the stk generators to compute samples independently of the number of channels of the, 
     *  VSCSynthSourceGenerator. These computed channels are then propagated to the of the VSCSynthSourceGenerator
     *  channels according to specified levels.
     *
     *  The number of channels of _computationFrames should not be set in this class, it will be different depending on 
     *  which stk class is used to compute the smaples (most generators would be mono, but file readers/loopers could be
     *  more i.e. stereo, 5.1 etc...)
     *
     */
    stk::StkFrames _computationFrames;
    /*
     * compute (call the tick method) mono frames, this is the actual computation of the samples 
     */
    virtual void processComputationFrames(void);
    /*
     * check that the number of frames in _computationFrames is the same as in argument
     */
    virtual void checkComputationFrames(stk::StkFrames& frames);
	
};

inline stk::StkFloat VSCSynthSourceGenerator::tick(void) {
    
    return 0.0;
    
}

inline stk::StkFrames& VSCSynthSourceGenerator::tick(stk::StkFrames& frames, unsigned int channel)
{
	return frames;
}

#endif // VSCS_USE_STK


#endif
