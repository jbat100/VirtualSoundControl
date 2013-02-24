

#ifndef _VSC_H_
#define _VSC_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <vector>

namespace VSC {
    
    typedef double                              Float;
    
    typedef std::vector<Float>                  Floats;
    
    typedef boost::posix_time::time_duration    TimeDuration;
    
    typedef boost::posix_time::ptime            Time;
    
    Time CurrentTime();
    
    /*
     *  Forward class definitions
     */
    
    class Environment;
    typedef boost::shared_ptr<Environment>  Environment_SPtr;
    typedef boost::weak_ptr<Environment>    Environment_WPtr;
	
}

#endif