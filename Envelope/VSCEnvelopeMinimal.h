
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
    class EnvelopeCoordinate;
    class EnvelopePoint;
    class EnvelopeEditorGUIConfig;
    class EnvelopeGUIConfig;
    
    typedef boost::shared_ptr<Envelope>     Envelope_SPtr;
    typedef boost::weak_ptr<Envelope>       Envelope_WPtr;
    typedef std::vector<Envelope_SPtr>      Envelopes;
    
    typedef boost::shared_ptr<EnvelopeCoordinate>           EnvelopeCoordinate_SPtr;
    typedef boost::shared_ptr<EnvelopePoint>                EnvelopePoint_SPtr;
    typedef boost::shared_ptr<EnvelopeEditorGUIConfig>      EnvelopeEditorGUIConfig_SPtr;
    typedef boost::shared_ptr<EnvelopeGUIConfig>            EnvelopeGUIConfig_SPtr;
    
    typedef std::vector<EnvelopePoint_SPtr>    EnvelopePoints;
    typedef std::set<EnvelopePoint_SPtr>       EnvelopePointSet;
    
}

#endif


