

#ifndef _VSC_H_
#define _VSC_H_

#include <boost/date_time/posix_time/posix_time.hpp>

#include <vector>

namespace VSC {
    
    typedef double                              Float;
    
    typedef std::vector<Float>                  Floats;
    
    typedef boost::posix_time::time_duration    TimeDuration;
    
    typedef boost::posix_time::ptime            Time;
    
    Time CurrentTime();
	
}

#endif