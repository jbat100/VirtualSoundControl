/*
 *  VSCSynthSourceSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_FILE_LOOP_
#define _VSC_SYNTH_SOURCE_FILE_LOOP_

#include "VSCSound.h"
#include "VSCSynthSourceGenerator.h"

#ifdef VSCS_USE_STK

#include "Stk.h"
#include "FileLoop.h"

class VSCSynthSourceFileLoop : public VSCSynthSourceGenerator {
	
public:
    
    stk::FileLoop* getStkFileLoop(void);
	
	void updateSoundEngine(void);
    
protected:
    
    stk::FileLoop fileLoop;
    void processComputationFrames(void);
	
};

inline void VSCSynthSourceFileLoop::processComputationFrames(void) {
	fileLoop.tick(_computationFrames);
}

#endif // #ifdef VSCS_USE_STK

#endif // #ifndef _VSC_SYNTH_SOURCE_FILE_LOOP_