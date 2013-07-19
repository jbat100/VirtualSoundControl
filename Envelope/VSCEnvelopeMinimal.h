
#ifndef _VSC_ENVELOPE__MINIMAL_H_
#define _VSC_ENVELOPE__MINIMAL_H_

#include "VSC.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>
#include <set>
#include <list>
#include <string>

namespace VSC {
    
    class Envelope;
    
    typedef boost::shared_ptr<Envelope>     Envelope_SPtr;
    typedef boost::weak_ptr<Envelope>       Envelope_WPtr;
    typedef std::vector<Envelope_SPtr>      Envelopes;
    
}

#endif


