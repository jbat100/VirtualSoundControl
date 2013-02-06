

#include "VSCIMAction.h"
#include "VSCIMActionImplementations.h"

#include "VSCMIDITasks.h"

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
            mImplementation = Action::Implementation::SPtr(new Action::ImplementationMIDINoteOn);
            break;
            
        case ActionTypeMIDINoteOff:
            mImplementation = Action::Implementation::SPtr(new Action::ImplementationMIDINoteOff);
            break;
            
        case ActionTypeMIDINoteOnAndOff:
            mImplementation = Action::Implementation::SPtr(new Action::ImplementationMIDINoteOnAndOff);
            break;
            
        case ActionTypeMIDIControlChange:
            mImplementation = Action::Implementation::SPtr(new Action::ImplementationMIDIControlChange);
            break;
            
        default:
            mImplementation = Action::Implementation::SPtr();
            break;
    }
    
    switch (actionType)
    {
        case ActionTypeMIDINoteOn:
        case ActionTypeMIDINoteOff:
        case ActionTypeMIDINoteOnAndOff:
        case ActionTypeMIDIControlChange:
        {
            this->setTaskQueue(MIDI::SingletonMIDITaskQueue());
        }
            break;
            
        default:
            break;
    }
    
    BOOST_ASSERT(mImplementation);
    if (mImplementation)
    {
        mImplementation->setupMappings(this->shared_from_this());
    }
    
    mActionType = actionType;
}


