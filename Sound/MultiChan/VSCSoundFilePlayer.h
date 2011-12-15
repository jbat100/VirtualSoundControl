/*
 *  VSCSoundSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_FILE_
#define _VSC_SYNTH_SOURCE_FILE_

#include "VSCSound.h"
#include "VSCSoundGenerator.h"
#include <boost/shared_ptr.hpp>

#define VSCSoundFilePlayerPtr      boost::shared_ptr<VSCSoundFilePlayer>

#ifdef VSCS_USE_STK

#include "Stk.h"
#include "FileWvIn.h"

class VSCSoundFilePlayer : public VSCSoundGenerator {
	
public:
	
    stk::FileWvIn* getStkFileWvIn(void);
	void updateSoundEngine(void);
    
protected:
    
    stk::FileWvIn fileWvIn;
    void processComputationFrames(void);
	
};

inline void VSCSoundFilePlayer::processComputationFrames(void) {
	assert(_computationFrames.channels());
    fileWvIn.tick(_computationFrames);
}

#endif // #ifdef VSCS_USE_STK

#endif // #ifndef _VSC_SYNTH_SOURCE_FILE_