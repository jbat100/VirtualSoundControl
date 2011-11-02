/*
 *  VSCSound.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 22/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSound.h"

#include <cmath>

VSCSFloat linearToDBGain(VSCSFloat linearGain) {
    return 10.0*std::log10(linearGain);
}

VSCSFloat dBToLinearGain(VSCSFloat dBGain) {
    return 0.1*std::pow(10.0, dBGain);
}
