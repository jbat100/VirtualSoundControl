
#include "VSC.h"

#include <boost/date_time/posix_time/posix_time.hpp>

VSC::Time VSC::CurrentTime()
{
    return boost::posix_time::microsec_clock::universal_time();
}