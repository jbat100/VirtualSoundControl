

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
    ValueMap valueMap;
    
    BOOST_FOREACH(const Target& target, this->getRequiredMappingTargets())
    {
        Mapping::SPtr mapping = this->getMapping(trigger, target);
        Float value = mapping->mappedValue(trigger, payload);
        valueMap[target] = value;
    }
    
    return this->generateTasksWithValueMap(valueMap);
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
            mImplementation = Implementation::SPtr();
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


