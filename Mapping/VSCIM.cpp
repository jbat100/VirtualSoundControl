
#include "VSCIM.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign/std/set.hpp>
#include <boost/foreach.hpp>
#include <boost/bimap.hpp>
#include <boost/assert.hpp>
#include <boost/thread.hpp>

namespace VSC
{
    namespace IM
    {
        void InitialiseInternals(void);
        
        typedef boost::bimap<Target, std::string>       TargetDescriptionMap;
        typedef boost::bimap<MappingType, std::string>  MappingDescriptionMap;
        typedef boost::bimap<ActionType, std::string>   ActionDescriptionMap;
        typedef boost::bimap<Trigger, std::string>      TriggerDescriptionMap;
        
        typedef TargetDescriptionMap::value_type        TargetDescriptionPair;
        typedef MappingDescriptionMap::value_type       MappingDescriptionPair;
        typedef ActionDescriptionMap::value_type        ActionDescriptionPair;
        typedef TriggerDescriptionMap::value_type       TriggerDescriptionPair;
        
        TargetDescriptionMap    targetDescriptionMap;
        TriggerDescriptionMap   triggerDescriptionMap;
        MappingDescriptionMap   mappingDescriptionMap;
        ActionDescriptionMap    actionDescriptionMap;
        
        typedef std::map<Trigger, MappingTypeSet> TriggerMappingTypeMap;
        
        TriggerMappingTypeMap   triggerMappingTypeMap;
        
        Triggers                allowedTriggers;
        
        ActionTypeSet           allowedActionTypeSet;
    }
}

using namespace VSC;
using namespace VSC::IM;
using namespace boost::assign;

boost::once_flag initializedInternalsFlag = BOOST_ONCE_INIT;

VSC::IM::TargetDescriptionMap targetDescriptionMap;
VSC::IM::MappingDescriptionMap mappingDescriptionMap;
VSC::IM::ActionDescriptionMap actionDescriptionMap;

VSC::IM::TriggerMappingTypeMap triggerMappingTypeMap;
VSC::IM::Triggers allowedTriggers;

VSC::IM::ActionTypeSet allowedActionTypeSet;


void VSC::IM::InitialiseInternals(void)
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
    
    triggerDescriptionMap.insert(TriggerDescriptionPair(TriggerNone, "No Trigger"));
    triggerDescriptionMap.insert(TriggerDescriptionPair(TriggerPlain, "Plain"));
    triggerDescriptionMap.insert(TriggerDescriptionPair(TriggerCollision, "Collision"));
    triggerDescriptionMap.insert(TriggerDescriptionPair(TriggerProximity, "Proximity"));
    
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeNone, "No Mapping"));
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeConstant, "Constant"));
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeCollisionVelocity, "Collision Velocity"));
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeCollisionDistance, "Collision Distance"));
    
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeNone, "No Action"));
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeVoid, "Void"));
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeMIDINoteOn, "MIDI Note On"));
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeMIDINoteOnAndOff, "MIDI Note On And Off"));
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeMIDINoteOff, "MIDI Note Off"));
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeMIDIControlChange, "MIDI Control Change"));
    
    MappingTypeSet plainSet;
    plainSet += MappingTypeConstant;
    triggerMappingTypeMap[TriggerPlain] = plainSet;
    
    MappingTypeSet collisionSet;
    collisionSet += MappingTypeConstant, MappingTypeCollisionVelocity, MappingTypeCollisionDistance;
    triggerMappingTypeMap[TriggerCollision] = collisionSet;
    
    MappingTypeSet proximitySet;
    proximitySet += MappingTypeConstant;
    triggerMappingTypeMap[TriggerProximity] = proximitySet;
    
    allowedTriggers += TriggerPlain, TriggerCollision, TriggerProximity;
    
    allowedActionTypeSet += ActionTypeVoid;
    allowedActionTypeSet += ActionTypeMIDINoteOn, ActionTypeMIDINoteOnAndOff, ActionTypeMIDINoteOff, ActionTypeMIDIControlChange;
}

const MappingTypeSet& VSC::IM::AllowedMappingTypeSetForTrigger(Trigger trigger)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    return triggerMappingTypeMap[trigger];
}

const Triggers& VSC::IM::AllowedTriggers(void)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    return allowedTriggers;
}

const ActionTypeSet& VSC::IM::AllowedActionTypes()
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    return allowedActionTypeSet;
}

std::string VSC::IM::StringForTrigger(const Trigger trigger)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    std::string s = triggerDescriptionMap.left.at(trigger);
    BOOST_ASSERT_MSG(!s.empty(), "Unexpected: Unknown Trigger");
    if (s.empty()) s = "Unknown Trigger";
    return s;
}

const Trigger VSC::IM::TriggerForString(const std::string& s)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    Trigger t = triggerDescriptionMap.right.at(s);
    return t;
}

std::string VSC::IM::StringForActionType(const ActionType actionType)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    std::string s = actionDescriptionMap.left.at(actionType);
    BOOST_ASSERT_MSG(!s.empty(), "Unexpected: Unknown ActionType");
    if (s.empty()) s = "Unknown Action Type";
    return s;
}

ActionType VSC::IM::ActionTypeForString(const std::string& s)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    ActionType t = actionDescriptionMap.right.at(s);
    return t;
}

std::string VSC::IM::StringForMappingType(const MappingType mappingType)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    std::string s = mappingDescriptionMap.left.at(mappingType);
    BOOST_ASSERT_MSG(!s.empty(), "Unexpected: unknown MappingType");
    if (s.empty()) s = "Unknown Mapping Type";
    return s;
}

MappingType VSC::IM::MappingTypeForString(const std::string& s)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    MappingType t = mappingDescriptionMap.right.at(s);
    return t;
}


std::string VSC::IM::StringForTarget(const Target target)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    std::string s = targetDescriptionMap.left.at(target);
    BOOST_ASSERT_MSG(!s.empty(), "Unexpected: Unknown Target");
    if (s.empty()) s = "Unknown Target";
    return s;
}

const Target VSC::IM::TargetForString(const std::string& s)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    Target t = targetDescriptionMap.right.at(s);
    return t;
}

