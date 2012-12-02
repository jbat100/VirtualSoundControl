

#include "VSCIMDelay.h"

#include <boost/date_time/posix_time/posix_time.hpp>

VSC::IM::Delay::Delay(const Float& seconds)
{
    mDelay = boost::posix_time::seconds(seconds);
}


