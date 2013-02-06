
#include "VSCIMEvent.h"
#include "VSCIMMapping.h"

#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;

//MARK: - Mapping Requirements

bool Event::checkRequiredMappings(void)
{
    BOOST_FOREACH(const Target& target, mRequiredMappingTargets)
    {
        if (!this->getMappingForTarget(target)) return false;
    }
    return true;
}

void Event::addRequiredMappingTarget(Target target)
{
    Targets::iterator it = std::find(mRequiredMappingTargets.begin(), mRequiredMappingTargets.end(), target);
    if (it == mRequiredMappingTargets.end()) mRequiredMappingTargets.push_back(target);
}

//MARK: - Mapping Getters

Mapping::SPtr Event::getMappingForTarget(Target target)
{
    return mMappingMap[target];
}

Mapping::SPtr Event::getCollisionMappingForTarget(Target target)
{
    return mCollisionMappingMap[target];
}

//MARK: - Mapping Setters

void Event::setMappingForTarget(Mapping::SPtr mapping, Target target)
{
    mMappingMap[target] = mapping;
}

void Event::setCollisionMappingForTarget(Mapping::SPtr mapping, Target target)
{
    mCollisionMappingMap[target] = mapping;
}

void Event::clearRequiredMappingTargets(void)
{
    mMappingMap.clear();
    mCollisionMappingMap.clear();
}