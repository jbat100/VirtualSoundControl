/*
 *  VSCSynthSourceSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_SQUARE_
#define _VSC_SYNTH_SOURCE_SQUARE_

#include "VSCSound.h"
#include "VSCSynthSourceGenerator.h"
#include <boost/shared_ptr.hpp>

#define VSCSynthSourceSquarePtr      boost::shared_ptr<VSCSynthSourceSquare>

#ifdef VSCS_USE_STK

#include "BlitSquare.h"

class VSCSynthSourceSquare : public VSCSynthSourceGenerator {
	
public:
    
    stk::BlitSquare* getStkBlitSquare(void);
	
	void updateSoundEngine(void);
	
protected:
    
	stk::BlitSquare blitSquare;
    
    void processComputationFrames(void);
	
};

inline void VSCSynthSourceSquare::processComputationFrames(void) {
	
}

#endif

#endif