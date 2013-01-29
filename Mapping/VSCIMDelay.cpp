
#include "VSCIMDelay.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

VSC::IM::Delay::Delay(void)
{
    this->addRequiredMappingTarget(TargetDuration);
}

void VSC::IM::Delay::createDefaultMappings(void)
{
    Mapping::SPtr defaultMapping = Mapping::SPtr(new Mapping);
    defaultMapping->setOffset(1.0);
    this->setMappingForTarget(defaultMapping, TargetDuration);
    
    Mapping::SPtr collisionMapping = Mapping::SPtr(new Mapping);
    collisionMapping->setOffset(1.0);
    this->setCollisionMappingForTarget(collisionMapping, TargetDuration);
}

VSC::TimeDuration VSC::IM::Delay::getDuration(void)
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

VSC::TimeDuration VSC::IM::Delay::getDurationForCollision(OB::Collision_SPtr collision, OB::Element_SPtr effector)
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


