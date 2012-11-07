//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_TARGET_H_
#define _VSC_IM_TARGET_H_

#include "VSC.h"

#include <string>
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
        
        std::string stringForTarget(const Target target);
        const Target targetForString(const std::string& s);

    }
    
}

#endif // _VSC_IM_TARGET_H_
