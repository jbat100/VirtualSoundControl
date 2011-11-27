//
//  VSCSynthSourceGenerator.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/2/11.
//  Copyright (c) 2011 NXP. All rights reserved.
//

#ifndef _VSC_SYNTH_SOURCE_GENERATOR_
#define _VSC_SYNTH_SOURCE_GENERATOR_

#include "VSCSound.h"
#include "VSCSoundParameters.h"
#include "VSCSoundMultiChannelElement.h"
#include "VSCSTKUtils.h"

#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>

#ifdef VSCS_USE_STK

#include "Stk.h"
#include "Generator.h"

#define VSCSynthSourceGeneratorPtr      boost::shared_ptr<VSCSynthSourceGenerator>

#define ConstSynthSrcGenIter            std::list<VSCSynthSourceGeneratorPtr>::const_iterator 
#define ConstRevSynthSrcGenIter         std::list<VSCSynthSourceGeneratorPtr>::const_reverse_iterator 

#define SynthSrcGenIter                 std::list<VSCSynthSourceGeneratorPtr>::iterator 
#define RevSynthSrcGenIter              std::list<VSCSynthSourceGeneratorPtr>::reverse_iterator 

class VSCSynthSourceGenerator : public VSCSoundMultiChannelElement, public stk::Generator {
    
    //class VSCSynthSourceGroup : public stk::Generator {
    
public:
	
	VSCSynthSourceGenerator();
	
    /*
     *  stk::Generator tick method which should be over-ridden.
     */
	stk::StkFrames& tick(stk::StkFrames& frames, unsigned int channel = 0);
	
	
	// StkFloat tick( void ); // This method is not actually declared in stk::Generator
	
	/*
	 *	Get the group generator to which this generator belongs, NULL if does not
	 *	belong to a group 
	 */
	VSCSynthSourceGenerator* getGroup(void);
	void setGroup(VSCSynthSourceGenerator* g);
	
	/*
	 *   VSCSoundMultiChannelElement methods
	 */
	virtual void setNumberOfChannels(unsigned int c);
	virtual VSCSFloat getValueForParameterWithKey(VSCSParameter::Key k);
	virtual void setValueForParameterWithKey(double val, VSCSParameter::Key k);
	
	/*
	 *	Sound engine methods
	 */
	virtual void initialize(void);
    virtual void updateSoundEngine(void);
	virtual void setIsOn(bool o);
	bool isOn(void);
	
	
	/*
	 *	Used for debugging and tracing, no encapsulation ...
	 */
	stk::StkFrames _pastFrames;
	unsigned int _numberOfPastFrames;
	bool _fillPastFrames;
	unsigned long long _tickCount;
	
protected:
	
	bool _isOn;
	std::string generatorType;
	
	/* 
     *  Keep a seperate StkFrames so that it does not need to be created everytime the tick function is called
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
	unsigned int _numberOfChannelsNeededForComputationFrames;
    
    /*
     *  Keep a StkFrames object for averaging (used when multi-channel _computationFrames are generated and the number 
     *  of channels does not correspond to _numberOfChannels) : the _computationFrames averaged over all channels are then assigned
     *  to all frames channels.
     */
    stk::StkFrames _averageFrames;
    
    /*
     * compute (call the tick method) mono frames, this is the actual computation of the samples 
     */
    virtual void processComputationFrames(unsigned int numberOfFrames);
	
	/*
	 *	Backpointer to the group generator which possesses this as subgenerator, NULL if it does not
	 *	belong to a group
	 */
	VSCSynthSourceGenerator* _group;
	
};

/*
 *  Don't use the plain sample tick because it just makes things more confusing, we can just use the frames 
 *  version below with 1 frame long argument for single multi-channel ticks. Using this will crash in debug,
 *  and output 0 in release.
 */
/*
inline stk::StkFloat VSCSynthSourceGenerator::tick(void) {
    assert(false);
    return 0.0;
}
 */



#endif // VSCS_USE_STK


#endif
