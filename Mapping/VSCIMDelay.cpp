
#include "VSCIMDelay.h"
#include "VSCIMMapping.h"
#include "VSCOBCollision.h"
#include "VSCOBElement.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

Delay::Delay(void)
{
    this->addRequiredMappingTarget(TargetDuration);
    
    this->generateMappings();
    
    BOOST_FOREACH(const Trigger& trigger, AllowedTriggers())
    {
        Mapping::SPtr mapping = this->getMapping(trigger, TargetDuration);
        mapping->setMappingType(MappingTypeConstant);
    }
}


TimeDuration Delay::getDuration(Trigger trigger, TriggerPayload::SPtr payload)
{
    Mapping::SPtr mapping = this->getMapping(trigger, TargetDuration);
    
    Float seconds = 0.0;
    
    BOOST_ASSERT(mapping);
    if (mapping)
    {
        seconds = mapping->mappedValue(trigger, payload);
    }
    
    Float milliseconds = seconds * 1000.0;
    
    return boost::posix_time::milliseconds(milliseconds);
}


