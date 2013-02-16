

#include "VSCIMAction.h"
#include "VSCIMActionImplementations.h"

#include "VSCMIDITasks.h"

#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

//MARK: - Task Generation


const Tasks Action::generateTasks(Trigger trigger, TriggerPayload::SPtr payload)
{
    TargetValueMap valueMap;
    
    BOOST_FOREACH(const Target& target, this->getRequiredMappingTargets())
    {
        Float value = 0.0;
        Mapping::SPtr mapping = this->getMapping(trigger, target);
        if (mapping)
        {
            value = mapping->mappedValue(trigger, payload);
        }
        else
        {
            std::cerr << "No mapping for target " << StringForTarget(target) << std::endl;
        }
        BOOST_ASSERT(mapping);
        valueMap[target] = value;
    }
    
    return this->generateTasksWithTargetValueMap(valueMap);
}

const Tasks Action::generateTasksWithTargetValueMap(Event::TargetValueMap& valueMap)
{
    Tasks tasks;
    return tasks;
}

void Action::setActionType(ActionType actionType)
{
    
    this->clearRequiredMappingTargets();
    this->clearMappings();
    
    switch (actionType)
    {
        case ActionTypeVoid:
            mImplementation = Implementation::SPtr(new ImplementationVoid);
            break;
            
        case ActionTypeMIDINoteOn:
            mImplementation = Implementation::SPtr(new ImplementationMIDINoteOn);
            this->addRequiredMappingTarget(TargetPitch);
            this->addRequiredMappingTarget(TargetVelocityOn);
            break;
            
        case ActionTypeMIDINoteOff:
            mImplementation = Implementation::SPtr(new ImplementationMIDINoteOff);
            this->addRequiredMappingTarget(TargetPitch);
            this->addRequiredMappingTarget(TargetVelocityOff);
            break;
            
        case ActionTypeMIDINoteOnAndOff:
            mImplementation = Implementation::SPtr(new ImplementationMIDINoteOnAndOff);
            this->addRequiredMappingTarget(TargetPitch);
            this->addRequiredMappingTarget(TargetVelocityOn);
            this->addRequiredMappingTarget(TargetDuration);
            this->addRequiredMappingTarget(TargetVelocityOff);
            break;
            
        case ActionTypeMIDIControlChange:
            mImplementation = Implementation::SPtr(new ImplementationMIDIControlChange);
            this->addRequiredMappingTarget(TargetValue);
            break;
            
        default:
            mImplementation = Implementation::SPtr();
            break;
    }
    
    /*
     *  Specific task queues
     */
    
    switch (actionType)
    {
        case ActionTypeMIDINoteOn:
        case ActionTypeMIDINoteOff:
        case ActionTypeMIDINoteOnAndOff:
        case ActionTypeMIDIControlChange:
        {
            this->setTaskQueue(MIDI::SingletonMIDITaskQueue());
            break;
        }
        default:
            break;
    }
    
    this->generateMappings();
    
    BOOST_ASSERT(mImplementation);
    if (mImplementation)
    {
        mImplementation->setupMappings(this->shared_from_this());
    }
    
    mActionType = actionType;
}


