

#include "VSCIMAction.h"

#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

//MARK: - Task Generation

const Tasks Action::generateTasks()
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

const Tasks Action::generateTasksForCollision(Collision_SPtr collision, Element_SPtr effector)
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
    
    BOOST_ASSERT(mImplementation);
    
    if (mImplementation)
    {
        mImplementation->generateTasksWithValueMap(valueMap);
    }
    
    Tasks tasks;
    
    return tasks;
}


void Action::setActionType(ActionType actionType)
{
    switch (actionType)
    {
        case ActionTypeMIDINoteOn:
            mImplementation = Implementation::SPtr(new ImplementationMIDINoteOn);
            break;
            
        case ActionTypeMIDINoteOff:
            mImplementation = Implementation::SPtr(new ImplementationMIDINoteOff);
            break;
            
        case ActionTypeMIDINoteOnAndOff:
            mImplementation = Implementation::SPtr(new ImplementationMIDINoteOnAndOff);
            break;
            
        case ActionTypeMIDIControlChange:
            mImplementation = Implementation::SPtr(new ImplementationMIDIControlChange);
            break;
            
        default:
            break;
    }
    
    mActionType = actionType;
}



const Tasks Action::ImplementationMIDINoteOn::generateTasksWithValueMap(Event::ValueMap& valueMap)
{
    
}


const Tasks Action::ImplementationMIDINoteOff::generateTasksWithValueMap(Event::ValueMap& valueMap)
{
    
}


const Tasks Action::ImplementationMIDINoteOnAndOff::generateTasksWithValueMap(Event::ValueMap& valueMap)
{
    
}

const Tasks Action::ImplementationMIDIControlChange::generateTasksWithValueMap(Event::ValueMap& valueMap)
{
    
}



