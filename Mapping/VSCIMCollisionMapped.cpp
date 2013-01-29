
#include "VSCIMCollisionMapped.h"

#include <boost/foreach.hpp>

bool VSC::IM::CollisionMapped::checkExpectedMappingTargets(void)
{
    BOOST_FOREACH(const Target& target, mExpectedTargets)
    {
        if (!this->getMappingForTarget(target)) return false;
    }
    return true;
}

void VSC::IM::CollisionMapped::addExpectedMappingTarget(Target target)
{
    Targets::iterator it = std::find(mExpectedTargets.begin(), mExpectedTargets.end(), target);
    if (it == mExpectedTargets.end()) mExpectedTargets.push_back(target);
}

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

VSC::IM::Target VSC::IM::CollisionMapped::getTargetForMapping(CollisionMapping::SPtr mapping)
{
    BOOST_FOREACH(const CollisionMappingMap::value_type& mapValue, mCollisionMappingMap)
    {
        if (mapValue.second == mapping) return mapValue.first;
    }
    
    return TargetNone;
}