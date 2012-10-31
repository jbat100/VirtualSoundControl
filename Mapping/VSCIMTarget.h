//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_TARGET_H_
#define _VSC_IM_TARGET_H_

#include "VSC.h"

#include <vector>

namespace VSC {
    
    namespace IM {
        
        enum Target {
            TargetNone = 0,
            TargetChannel,
            TargetPitch,
            TargetVelocityOn,
            TargetVelocityOff,
            TargetDuration,
            TargetControlValue
        };
        
        typedef std::vector<Target> Targets;

    }
    
}

#endif // _VSC_IM_TARGET_H_
