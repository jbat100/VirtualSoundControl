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

#define VSCS_USE_STK

#define VSCS_USE_DOUBLE_PRECISION


//#ifdef VSCS_USE_STK
	#include "Stk.h"
	//Define VSCSFloat (double or float or same as stk::StkFloat)
	typedef stk::StkFloat VSCSFloat;
//#else 
//	typedef double VSCSFloat;
//#endif


#endif


