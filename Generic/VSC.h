//
//  VSCMath.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 8/9/11.
//  Copyright 2011 NXP. All rights reserved.
//

#ifndef _VSC_MATH_H_
#define _VSC_MATH_H_

#include <boost/date_time/posix_time/posix_time.hpp>

namespace VSC {
    
    typedef double                              Float;
    
    typedef boost::posix_time::time_duration    TimeDuration;
    
    typedef boost::posix_time::ptime            Time;
    
    Time CurrentTime();
	
}

#endif