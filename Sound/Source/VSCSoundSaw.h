/*
 *  VSCSoundSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_SAW_
#define _VSC_SYNTH_SOURCE_SAW_

#include "VSCSound.h"
#include "VSCSoundGenerator.h"
#include <boost/shared_ptr.hpp>

#define VSCSoundSawPtr      boost::shared_ptr<VSCSoundSaw>

#ifdef VSCS_USE_STK

#include "BlitSaw.h"

class VSCSoundSaw : public VSCSoundGenerator {
	
public:
	
	virtual VSCSFloat tick(void);
    
    stk::BlitSaw& getStkBlitSaw(void);
	
private:
    
	stk::BlitSaw blitSaw;
	
};

inline VSCSFloat VSCSoundSaw::tick(void) {
	return blitSaw.tick();
}

#endif

#endif