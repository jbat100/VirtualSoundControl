

#include "VSCIMAction.h"

#include <boost/foreach.hpp>

//MARK: - Task Generation

VSC::Tasks VSC::IM::Action::generateTasks()
{
    Tasks tasks;
    return tasks;
}

VSC::Tasks VSC::IM::Action::generateTasksForCollision(OB::Collision_SPtr collision, OB::Element_SPtr effector)
{
    Tasks tasks;
    return tasks;
}

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

VSC::IM::CollisionMapping::SPtr VSC::IM::Action::getCollisionMappingForTarget(Target target)
{
    return mCollisionMappingMap[target];
}

//MARK: - Mapping Setters

void VSC::IM::Action::setMappingForTarget(Mapping::SPtr mapping, Target target)
{
    mMappingMap[target] = mapping;
}

void VSC::IM::Action::setCollisionMappingForTarget(CollisionMapping::SPtr mapping, Target target)
{
    mCollisionMappingMap[target] = mapping;
}

