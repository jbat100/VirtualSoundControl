/*
 *  VSCSoundSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_SINE_
#define _VSC_SYNTH_SOURCE_SINE_

#include "VSCSound.h"
#include "VSCSoundGenerator.h"
#include <boost/shared_ptr.hpp>

#define VSCSoundSinePtr      boost::shared_ptr<VSCSoundSine>

#ifdef VSCS_USE_STK

#include "Stk.h"
#include "SineWave.h"


class VSCSoundSine : public VSCSoundGenerator {
	
public:
	
	/*
	 *	Default constructor
	 */
	VSCSoundSine();
	
	virtual double getValueForParameterWithKey(VSCSParameter::Key key);
	virtual void setValueForParameterWithKey(double value, VSCSParameter::Key key);
	
	/*
	 *	Setters/Getters
	 */
	VSCSFloat getFrequency(void);
	void setFrequency(VSCSFloat f);
	
	VSCSFloat getPhase(void);
	void setPhase(VSCSFloat f);
	void addPhase(VSCSFloat f);
    
	/*
	 *	STK engine (shouldn't use apart from debug...)
	 */
    const stk::SineWave& getStkSineWave(void);
    
protected:
    
    virtual void processComputationFrames(unsigned int numberOfFrames);
	
private:
	
	static const std::string kSynthSourceSineType;
    
	stk::SineWave sineWave;
	
	/*
	 *	For some stupid reason the guys at STK have decided not to make it possible to know the 
	 *	frequency and phase of the SineWave, so we store it here... WTF... why...
	 */
	VSCSFloat _frequency;
	VSCSFloat _phase;
	
};




#endif

#endif