/*
 *  VSCSoundSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_SQUARE_
#define _VSC_SYNTH_SOURCE_SQUARE_

#include "VSCSound.h"
#include "VSCSoundGenerator.h"
#include <boost/shared_ptr.hpp>

#define VSCSoundSquarePtr      boost::shared_ptr<VSCSoundSquare>

#ifdef VSCS_USE_STK

#include "BlitSquare.h"

class VSCSoundSquare : public VSCSoundGenerator {
	
public:
	
	VSCSoundSquare();
    
    stk::BlitSquare& getStkBlitSquare(void);
	
protected:
    
	stk::BlitSquare blitSquare;
    
    void processComputationFrames(void);
	
};

#endif

#endif