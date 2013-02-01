
#include "VSCIM.h"
#include "VSCIMAction.h"
#include "VSCIMMIDIActions.h"
#include "VSCIMCollisionMapping.h"

VSC::IM::ActionType VSC::IM::actionTypeForAction(Action::SPtr action)
{
    if (action)
    {
        if (boost::dynamic_pointer_cast<MIDINoteOnAction>(action))
            return ActionTypeMIDINoteOn;
        
        if (boost::dynamic_pointer_cast<MIDINoteOnAndOffAction>(action))
            return ActionTypeMIDINoteOnAndOff;
        
        if (boost::dynamic_pointer_cast<MIDINoteOffAction>(action))
            return ActionTypeMIDINoteOff;
        
        if (boost::dynamic_pointer_cast<MIDIControlChangeAction>(action))
            return ActionTypeMIDIControlChange;
    }
    
    return ActionTypeNone;
}

VSC::IM::Action::SPtr VSC::IM::createActionWithType(ActionType actionType)
{
    switch (actionType)
    {
        case ActionTypeMIDINoteOn:
            return Action::SPtr(new MIDINoteOnAction);
            
        case ActionTypeMIDINoteOnAndOff:
            return Action::SPtr(new MIDINoteOnAndOffAction);
            
        case ActionTypeMIDINoteOff:
            return Action::SPtr(new MIDINoteOffAction);
            
        case ActionTypeMIDIControlChange:
            return Action::SPtr(new MIDIControlChangeAction);
            
        default:
            break;
    }
    
    return Action::SPtr();
}

std::string VSC::IM::stringForActionType(ActionType actionType)
{
    switch (actionType)
    {
        case ActionTypeMIDINoteOn:
            return "MIDI Note On";
            
        case ActionTypeMIDINoteOnAndOff:
            return "MIDI Note On And Off";
            
        case ActionTypeMIDINoteOff:
            return "MIDI Note Off";
            
        case ActionTypeMIDIControlChange:
            return "MIDI Control Change";
            
        default:
            break;
    }
    
    return "Unknown Action";
}

VSC::IM::MappingType VSC::IM::mappingTypeForMapping(Mapping::SPtr mapping)
{
    if (mapping)
    {
        if (boost::dynamic_pointer_cast<Mapping>(mapping))
            return MappingTypeConstant;
        
        if (boost::dynamic_pointer_cast<CollisionVelocityMapping>(mapping))
            return MappingTypeCollisionVelocity;
        
        if (boost::dynamic_pointer_cast<CollisionDistanceMapping>(mapping))
            return MappingTypeCollisionDistance;
    }
    
    return MappingTypeNone;
}

VSC::IM::Mapping::SPtr VSC::IM::createMappingWithType(MappingType mappingType)
{
    switch (mappingType)
    {
        case MappingTypeConstant:
            return Mapping::SPtr(new Mapping);
            
        case MappingTypeCollisionVelocity:
            return Mapping::SPtr(new CollisionVelocityMapping);
            
        case MappingTypeCollisionDistance:
            return Mapping::SPtr(new CollisionDistanceMapping);
            
        default:
            break;
    }
    
    return Mapping::SPtr();
}

std::string VSC::IM::stringForMappingType(MappingType mappingType)
{
    switch (mappingType)
    {
        case MappingTypeConstant:
            return "Constant";
            
        case MappingTypeCollisionVelocity:
            return "Collision Velocity";
            
        case MappingTypeCollisionDistance:
            return "Collision Distance";
            
        default:
            break;
    }
}
