

#include "VSCIMCollisionMapped.h"

#include <boost/foreach.hpp>

VSC::IM::CollisionMapping::SPtr VSC::IM::CollisionMapped::getMappingForTarget(Target target)
{
    return mCollisionMappingMap[target];
}

void VSC::IM::CollisionMapped::setMappingForTarget(Target target, CollisionMapping::SPtr mapping)
{
    mCollisionMappingMap[target] = mapping;
}

void VSC::IM::CollisionMapped::removeMappingForTarget(const Target target)
{
    mCollisionMappingMap.erase(target);
}

const VSC::IM::Targets VSC::IM::CollisionMapped::getMappingTargets()
{
    Targets targets;
    
    BOOST_FOREACH(const CollisionMappingMap::value_type& mapValue, mCollisionMappingMap)
    {
        targets.push_back(mapValue.first);
    }
    
    return targets;
}

VSC::IM::Target VSC::IM::CollisionMapped::getTargetForMapping(CollisionMapping::SPtr mapping)
{
    
}