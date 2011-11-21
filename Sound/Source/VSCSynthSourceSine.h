/*
 *  VSCSynthSourceSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_SINE_
#define _VSC_SYNTH_SOURCE_SINE_

#include "VSCSound.h"
#include "VSCSynthSourceGenerator.h"
#include <boost/shared_ptr.hpp>

#define VSCSynthSourceSinePtr      boost::shared_ptr<VSCSynthSourceSine>

#ifdef VSCS_USE_STK

#include "Stk.h"
#include "SineWave.h"


class VSCSynthSourceSine : public VSCSynthSourceGenerator {
	
public:
	
	/*
	 *	Default constructor
	 */
	VSCSynthSourceSine();
	
	virtual double getValueForParameterWithKey(VSCSParameter::Key key);
	virtual void setValueForParameterWithKey(double value, VSCSParameter::Key key);
    
	/*
	 *	STK engine (shouldn't use apart from debug...)
	 */
    stk::SineWave* getStkSineWave(void);
	
	/*
	 *	Call on change number of channels
	 */
	void updateSoundEngine(void);
	
protected:
    
	stk::SineWave sineWave;
	
	/*
	 *	Setters/Getters
	 */
	VSCSFloat getFrequency(void);
	void setFrequency(VSCSFloat f);
	
	VSCSFloat getPhase(void);
	void setPhase(VSCSFloat f);
	void addPhase(VSCSFloat f);
	
	/*
	 *	For some stupid reason the guys at STK have decided not to make it possible to know the 
	 *	frequency and phase of the SineWave, so we store it here... WTF... why...
	 */
	VSCSFloat _frequency;
	VSCSFloat _phase;
    
    void processComputationFrames(void);
	
};

inline void VSCSynthSourceSine::processComputationFrames(void) {
	sineWave.tick(_computationFrames);
}


#endif

#endif