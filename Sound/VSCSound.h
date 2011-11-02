/*
 *  VSCSound.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 22/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 *
 *	General definitions for everything that has to do with sound stuff
 *
 */


#ifndef _VSC_SOUND_H_
#define _VSC_SOUND_H_

#include <climits>

#define VSCS_USE_STK

/*
 *  Define whether the sound part of VSC (VSCS) should do processing in double or float (this should be in accordance with 
 *  the type of VSCSFloat). Useful for calling different Accelerate framework methods depending on precision for example.
 */
#define VSCS_USE_DOUBLE_PRECISION

/*
 *  Define VSCS_ALL_CHANNELS to specify audio processing tick is to occur for all channels 
 *  
 *  This is used in the stk::generator call
 *  stk::StkFrames& tick(stk::StkFrames& frames, unsigned int channel = 0);
 */
#define VSCS_ALL_CHANNELS UINT_MAX

#ifdef VSCS_USE_STK
	#include "Stk.h"
	//Define VSCSFloat (double or float or same as stk::StkFloat)
	typedef stk::StkFloat VSCSFloat;
#else 
	typedef double VSCSFloat;
#endif


VSCSFloat linearToDBGain(VSCSFloat linearGain);
VSCSFloat dBToLinearGain(VSCSFloat dBGain);

#endif


