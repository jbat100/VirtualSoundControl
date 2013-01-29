

#include "VSCIMAction.h"

#include <boost/foreach.hpp>

//MARK: - Task Generation

const VSC::Tasks VSC::IM::Action::generateTasks()
{
    ValueMap valueMap;
    
    BOOST_FOREACH(const Target& target, this->getRequiredMappingTargets())
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
    
    BOOST_FOREACH(const Target& target, this->getRequiredMappingTargets())
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



