

#ifndef _VSC_RANDOM_H_
#define _VSC_RANDOM_H_

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/generator_iterator.hpp>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <vector>

namespace VSC {
    
    /*
     *  This flexible MPL based multi-type random number generator template comes from here...
     *  http://stackoverflow.com/questions/6470074/boostrandom-and-boostuniform-real-works-with-doubles-not-with-floats
     */

    template <typename N>
    N GenerateRandomNumber(N min, N max)
    {
        typedef typename boost::mpl::if_<
        boost::is_floating_point<N>, // if we have a floating point type
        boost::uniform_real<>,       // use this, or
        boost::uniform_int<>         // else use this one
        >::type distro_type;
        
        timeval t;
        gettimeofday(&t, NULL);
        boost::mt19937 seed((int)t.tv_sec);
        distro_type dist(min, max);
        boost::variate_generator<boost::mt19937&, distro_type > random(seed, dist);
        return random(); 
    };
	
}

#endif //_VSC_RANDOM_H_
