/*
 *  VSCSTKUtils.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 01/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_STK_UTILS_H_
#define _VSC_STK_UTILS_H_

#include "Stk.h"
#include "Generator.h"
#include "FileWvIn.h"
#include "FileLoop.h"

#include "VSCSound.h"
#include "VSCSynthSourceElement.h"


stk::Generator* castVSCSynthSourceElementToStkGenerator(VSCSynthSourceElement* elem);
stk::FileWvIn* castVSCSynthSourceElementToStkFileWvIn(VSCSynthSourceElement* elem);
stk::FileLoop* castVSCSynthSourceElementToStkFileLoop(VSCSynthSourceElement* elem);

#endif
