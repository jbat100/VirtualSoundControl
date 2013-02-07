
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


VSC::TimeDuration Delay::getDuration(void)
{
    Mapping::SPtr mapping = this->getMappingForTarget(TargetDuration);
    
    Float seconds = 0.0;
    
    BOOST_ASSERT(mapping);
    if (mapping)
    {
        seconds = mapping->mappedValue();
    }
    
    Float milliseconds = seconds * 1000.0;
    
    return boost::posix_time::milliseconds(milliseconds);
}

VSC::TimeDuration Delay::getDurationForCollision(Collision_SPtr collision, Element_SPtr effector)
{
    Mapping::SPtr mapping = this->getCollisionMappingForTarget(TargetDuration);
    Float seconds = 0.0;
    
    BOOST_ASSERT(mapping);
    if (mapping)
    {
        CollisionMapping::SPtr collisionMapping = boost::dynamic_pointer_cast<CollisionMapping>(mapping);
        if (collisionMapping)
        {
            seconds = collisionMapping->mappedValue(collision, effector);
        }
        else
        {
            seconds = mapping->mappedValue();
        }
    }
    
    Float milliseconds = seconds * 1000.0;
    
    return boost::posix_time::milliseconds(milliseconds);
    
}


