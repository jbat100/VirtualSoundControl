
#include "VSCIMEvent.h"


//MARK: - Mapping Requirements

bool VSC::IM::Action::checkRequiredMappings(void)
{
    BOOST_FOREACH(const Target& target, mRequiredMappingTargets)
    {
        if (!this->getMappingForTarget(target)) return false;
    }
    return true;
}

void VSC::IM::Action::addRequiredMappingTarget(Target target)
{
    Targets::iterator it = std::find(mRequiredMappingTargets.begin(), mRequiredMappingTargets.end(), target);
    if (it == mRequiredMappingTargets.end()) mRequiredMappingTargets.push_back(target);
}

//MARK: - Mapping Getters

VSC::IM::Mapping::SPtr VSC::IM::Action::getMappingForTarget(Target target)
{
    return mMappingMap[target];
}

VSC::IM::Mapping::SPtr VSC::IM::Action::getCollisionMappingForTarget(Target target)
{
    return mCollisionMappingMap[target];
}

//MARK: - Mapping Setters

void VSC::IM::Action::setMappingForTarget(Mapping::SPtr mapping, Target target)
{
    mMappingMap[target] = mapping;
}

void VSC::IM::Action::setCollisionMappingForTarget(Mapping::SPtr mapping, Target target)
{
    mCollisionMappingMap[target] = mapping;
}