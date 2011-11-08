/*
 *  VSCSynthSourceSine.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 15/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSynthSourceFile.h"


stk::FileWvIn* VSCSynthSourceFile::getStkFileWvIn(void) {
    return &fileWvIn;
}
