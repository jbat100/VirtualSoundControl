//
//  VSCSoundGenerator.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/2/11.
//  Copyright (c) 2011 NXP. All rights reserved.
//

#ifndef _VSC_SYNTH_SOURCE_GENERATOR_
#define _VSC_SYNTH_SOURCE_GENERATOR_

#include "VSCSound.h"
#include "VSCSoundParameters.h"
#include "VSCSoundElement.h"
#include "VSCSTKUtils.h"

#include <list>
#include <vector>
#include <deque>
#include <boost/shared_ptr.hpp>


#define VSCSoundGeneratorPtr			boost::shared_ptr<VSCSoundGenerator>

#define ConstSynthSrcGenIter            std::list<VSCSoundGeneratorPtr>::const_iterator 
#define ConstRevSynthSrcGenIter         std::list<VSCSoundGeneratorPtr>::const_reverse_iterator 

#define SynthSrcGenIter                 std::list<VSCSoundGeneratorPtr>::iterator 
#define RevSynthSrcGenIter              std::list<VSCSoundGeneratorPtr>::reverse_iterator 

class VSCSoundGenerator : public VSCSoundElement {
    
    //class VSCSoundGeneratorGroup : public stk::Generator {
    
public:
	
	VSCSoundGenerator();
	
	virtual VSCSFloat tick(void); 
	
	/*
	 *	Get the group generator to which this generator belongs, NULL if does not
	 *	belong to a group 
	 */
	VSCSoundGenerator* getGroup(void);
	void setGroup(VSCSoundGenerator* g);
	
	/*
	 *   VSCSoundMultiChannelElement methods
	 */
	virtual double getValueForParameterWithKey(VSCSParameter::Key k);
	virtual void setValueForParameterWithKey(double val, VSCSParameter::Key k);
	
	/*
	 *	Sound engine methods
	 */
	virtual void initialize(void);
	virtual void setIsOn(bool o);
	bool isOn(void);
	
	/*
     *  Frame debugging
     */
    void tracePastSamples(unsigned int n);
	unsigned int traceSampleSize(void);
    const std::deque<VSCSFloat>& getPastSamples(void);
    unsigned long long getTickCount(void);
	
protected:
	
	void trace(VSCSFloat f);
    
private:
    
    bool _isOn;
    
    /*
	 *	Backpointer to the group generator which possesses this as subgenerator, NULL if it does not
	 *	belong to a group
	 */
	VSCSoundGenerator* _group;
    
    /*
	 *	Used for debugging and tracing, (does not affect any processing just logs values)
	 */
	std::deque<VSCSFloat> _pastSamples;
	unsigned int _traceSampleSize;
	unsigned long long _tickCount;
	
};




#endif
