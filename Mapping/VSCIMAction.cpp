

#include "VSCIMAction.h"

#include <boost/foreach.hpp>

//MARK: - Task Generation

const VSC::Tasks VSC::IM::Action::generateTasks()
{
    ValueMap valueMap;
    
    BOOST_FOREACH(const Target& target, mRequiredMappingTargets)
    {
        Mapping::SPtr mapping = this->getMappingForTarget(target);
        Float value = mapping->mappedValue();
        valueMap[target] = value;
    }
    
    return this->generateTasksWithValueMap(valueMap);
}

const VSC::Tasks VSC::IM::Action::generateTasksForCollision(OB::Collision_SPtr collision, OB::Element_SPtr effector)
{
    ValueMap valueMap;
    
    BOOST_FOREACH(const Target& target, mRequiredMappingTargets)
    {
        Float value = 0.0;
        Mapping::SPtr mapping = this->getCollisionMappingForTarget(target);
        
        CollisionMapping::SPtr collisionMapping = boost::dynamic_pointer_cast<CollisionMapping>(mapping);
        
        if (collisionMapping)
        {
            value = collisionMapping->mappedValue(collision, effector);
        }
        else
        {
            value = mapping->mappedValue();
        }
        
        valueMap[target] = value;
    }
    
    return this->generateTasksWithValueMap(valueMap);
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

