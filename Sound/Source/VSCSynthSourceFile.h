/*
 *  VSCSynthSourceSine.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SYNTH_SOURCE_FILE_
#define _VSC_SYNTH_SOURCE_FILE_

#include "VSCSound.h"
#include "VSCSynthSourceGenerator.h"

#ifdef VSCS_USE_STK

#include "Stk.h"
#include "FileWvIn.h"

class VSCSynthSourceFile : public VSCSynthSourceGenerator {
	
public:
	
    stk::FileWvIn* getStkFileWvIn(void);
    
protected:
    
    stk::FileWvIn fileWvIn;
    void processComputationFrames(void);
	
};

inline void VSCSynthSourceFile::processComputationFrames(void) {
    fileWvIn.tick(_computationFrames);
}

#endif // #ifdef VSCS_USE_STK

#endif // #ifndef _VSC_SYNTH_SOURCE_FILE_