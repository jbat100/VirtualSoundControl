/*
 *  VSCSynthInstrument.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 05/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_SYNTH_INSTRUMENT_
#define _VSC_SYNTH_INSTRUMENT_

#include "VSCSound.h"

#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>

#define VSCSynthInstrumentPtr		boost::shared_ptr<VSCSynthInstrument>

#define ConstSynthInstrmntIter		std::list<VSCSynthInstrumentPtr>::const_iterator 
#define ConstRevSynthInstrmntIter	std::list<VSCSynthInstrumentPtr>::const_reverse_iterator 

#define SynthSynthInstrmntIter		std::list<VSCSynthInstrumentPtr>::iterator 
#define RevSynthSynthInstrmntIter	std::list<VSCSynthInstrumentPtr>::reverse_iterator 


class VSCSynthInstrument {
	
	
public:
    
    VSCSynthInstrument();
	
	/*--------------------------------------------------------------*/
	
	/*
	 *	stk::Instrmnt like methods 
	 */
	
	/*
	 *	Note on/off messages
	 */
	// 0.0 < 0 < 127.0    0.0 < amplitude < 127.0
	virtual void noteOn(VSCSFloat note, VSCSFloat amplitude);
	virtual void noteOff(VSCSFloat amplitude); // stk::Instrmnt method (amplitude is used for decay time)
	
	//! Set instrument parameters for a particular frequency.
	virtual void setFrequency(VSCSFloat freq);
	VSCSFloat getFrequency(void);
	    
    /*--------------------------------------------------------------*/
    
    /*
     *  Perform initialization after constructor (seperate so that it can be virtual).
     */
    virtual void initialize(void);
    
    /*
     *  Perform necessary setup operations after:
     *      - a change in number of channels.
     */
    virtual void updateSoundEngine(void);
    
    /*
     *  A human readable identifier for the source element.
     */
	virtual std::string intrumentTypeString(void);
	
	/*--------------------------------------------------------------*/
	
	
private:
	
	bool _isOn;
	VSCSFloat _frequency;
	
};

#endif
