
#include "VSCIM.h"
#include "VSCIMAction.h"
#include "VSCIMMIDIActions.h"
#include "VSCIMCollisionMapping.h"

#include <boost/foreach.hpp>
#include <boost/bimap.hpp>

namespace VSC
{
    namespace IM
    {
        void InitialiseMaps(void);
        
        typedef boost::bimap<Target, std::string>       TargetDescriptionMap;
        typedef boost::bimap<MappingType, std::string>  MappingDescriptionMap;
        typedef boost::bimap<ActionType, std::string>   ActionDescriptionMap;
        
        typedef TargetDescriptionMap::value_type        TargetDescriptionPair;
        typedef MappingDescriptionMap::value_type       MappingDescriptionPair;
        typedef ActionDescriptionMap::value_type        ActionDescriptionPair;
        
        TargetDescriptionMap     targetDescriptionMap;
        MappingDescriptionMap    mappingDescriptionMap;
        ActionDescriptionMap     actionDescriptionMap;
    }
}

using namespace VSC;
using namespace VSC::IM;

boost::once_flag createdDescriptionsMapFlag = BOOST_ONCE_INIT;

VSC::IM::TargetDescriptionMap targetDescriptionMap;
VSC::IM::MappingDescriptionMap mappingDescriptionMap;
VSC::IM::ActionDescriptionMap actionDescriptionMap;


void InitialiseMaps(void)
{
    BOOST_ASSERT(targetDescriptionMap.empty());
    BOOST_ASSERT(mappingDescriptionMap.empty());
    BOOST_ASSERT(actionDescriptionMap.empty());
    
    targetDescriptionMap.insert(TargetDescriptionPair(TargetNone, "No Target"));
    targetDescriptionMap.insert(TargetDescriptionPair(TargetChannel, "Channel"));
    targetDescriptionMap.insert(TargetDescriptionPair(TargetPitch,  "Pitch"));
    targetDescriptionMap.insert(TargetDescriptionPair(TargetVelocityOn, "Velocity On"));
    targetDescriptionMap.insert(TargetDescriptionPair(TargetVelocityOff, "Velocity Off"));
    targetDescriptionMap.insert(TargetDescriptionPair(TargetDuration, "Duration"));
    targetDescriptionMap.insert(TargetDescriptionPair(TargetValue, "Control Value"));
    
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeNone, "No Mapping"));
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeConstant, "Constant"));
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeCollisionVelocity, "Collision Velocity"));
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeCollisionDistance, "Collision Distance"));
    
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeNone, "No Action"));
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeMIDINoteOn, "MIDI Note On"));
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeMIDINoteOnAndOff, "MIDI Note On And Off"));
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeMIDINoteOff, "MIDI Note Off"));
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeMIDIControlChange, "MIDI Control Change"));
    
}

ActionType actionTypeForAction(Action::SPtr action)
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

Action::SPtr createActionWithType(ActionType actionType)
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

std::string stringForActionType(const ActionType actionType)
{
    boost::call_once(&InitialiseMaps, createdDescriptionsMapFlag);
    std::string s = actionDescriptionMap.left.at(actionType);
    BOOST_ASSERT_MSG(s.empty(), "Unexpected: Unknown ActionType");
    if (s.empty()) s = "Unknown Action Type";
    return s;
}

ActionType actionTypeForString(const std::string& s)
{
    boost::call_once(&InitialiseMaps, createdDescriptionsMapFlag);
    ActionType t = actionDescriptionMap.right.at(s);
    return t;
}

std::string stringForMappingType(const MappingType mappingType)
{
    boost::call_once(&InitialiseMaps, createdDescriptionsMapFlag);
    std::string s = actionDescriptionMap.left.at(actionType);
    BOOST_ASSERT_MSG(s.empty(), "Unexpected: unknown MappingType");
    if (s.empty()) s = "Unknown Mapping Type";
    return s;
}

MappingType mappingTypeForString(const std::string& s)
{
    boost::call_once(&InitialiseMaps, createdDescriptionsMapFlag);
    MappingType t = mappingDescriptionMap.right.at(s);
    return t;
}


std::string stringForTarget(const Target target)
{
    boost::call_once(&InitialiseMaps, createdDescriptionsMapFlag);
    std::string s = targetDescriptionMap.left.at(target);
    BOOST_ASSERT_MSG(s.empty(), "Unexpected: Unknown Target");
    if (s.empty()) s = "Unknown Target";
    return s;
}

const Target targetForString(const std::string& s)
{
    boost::call_once(&InitialiseMaps, createdDescriptionsMapFlag);
    Target t = targetDescriptionMap.right.at(s);
    return t;
}

