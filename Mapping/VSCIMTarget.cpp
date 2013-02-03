
#include "VSCIMTarget.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>

#include <map>

boost::once_flag createdTargetDescriptionMapFlag = BOOST_ONCE_INIT;

namespace VSC {
    namespace IM {
        void InitialiseTargetDescriptionMap(void);
        typedef std::map<Target, std::string> TargetDescriptionMap;
        static TargetDescriptionMap targetDescriptionMap;
    }
}

VSC::IM::TargetDescriptionMap targetDescriptionMap;

void VSC::IM::InitialiseTargetDescriptionMap(void)
{
    BOOST_ASSERT(targetDescriptionMap.empty());
    
    targetDescriptionMap[TargetNone]            = "No Target";
    targetDescriptionMap[TargetChannel]         = "Channel";
    targetDescriptionMap[TargetPitch]           = "Pitch";
    targetDescriptionMap[TargetVelocityOn]      = "Velocity On";
    targetDescriptionMap[TargetVelocityOff]     = "Velocity Off";
    targetDescriptionMap[TargetDuration]        = "Duration";
    targetDescriptionMap[TargetValue]    = "Control Value";
    
}

std::string VSC::IM::stringForTarget(const Target target)
{
    boost::call_once(&InitialiseTargetDescriptionMap, createdTargetDescriptionMapFlag);
    
    return targetDescriptionMap[target];
}

const VSC::IM::Target VSC::IM::targetForString(const std::string& s)
{
    boost::call_once(&InitialiseTargetDescriptionMap, createdTargetDescriptionMapFlag);
    
    BOOST_FOREACH(const TargetDescriptionMap::value_type& mapValue, targetDescriptionMap)
    {
        const std::string& desc = mapValue.second;
        if (s.compare(desc) == 0)
        {
            return mapValue.first;
        }
    }
    
    return TargetNone;
}


