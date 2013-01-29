
#include "VSCIMActionTypes.h"
#include "VSCIMAction.h"
#include "VSCIMMIDIActions.h"

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
}

