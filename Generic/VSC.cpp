//
//  VSCMath.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 8/9/11.
//  Copyright 2011 NXP. All rights reserved.
//

#include "VSC.h"

#include <boost/date_time/posix_time/posix_time.hpp>

VSC::Time VSC::CurrentTime()
{
    return boost::posix_time::microsec_clock::universal_time();
}