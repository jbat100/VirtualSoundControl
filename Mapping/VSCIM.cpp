
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
        
        typedef TargetDescriptionMap::value_type        TargetDescriptionPair;
        typedef MappingDescriptionMap::value_type       MappingDescriptionPair;
        typedef ActionDescriptionMap::value_type        ActionDescriptionPair;
        
        TargetDescriptionMap    targetDescriptionMap;
        MappingDescriptionMap   mappingDescriptionMap;
        ActionDescriptionMap    actionDescriptionMap;
        
        typedef std::map<Trigger, MappingTypeSet> TriggerMappingTypeMap;
        
        TriggerMappingTypeMap   triggerMappingTypeMap;
        
        Triggers                allowedTriggers;
    }
}

using namespace VSC;
using namespace VSC::IM;
using namespace boost::assign;

boost::once_flag initializedInternalsFlag = BOOST_ONCE_INIT;

VSC::IM::TargetDescriptionMap targetDescriptionMap;
VSC::IM::MappingDescriptionMap mappingDescriptionMap;
VSC::IM::ActionDescriptionMap actionDescriptionMap;


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
    
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeNone, "No Mapping"));
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeConstant, "Constant"));
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeCollisionVelocity, "Collision Velocity"));
    mappingDescriptionMap.insert(MappingDescriptionPair(MappingTypeCollisionDistance, "Collision Distance"));
    
    actionDescriptionMap.insert(ActionDescriptionPair(ActionTypeNone, "No Action"));
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
}

const MappingTypeSet& AllowedMappingTypeSetForTrigger(Trigger trigger)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    return triggerMappingTypeMap[trigger];
}

const Triggers& AllowedTriggers(void)
{
    return allowedTriggers;
}

std::string StringForActionType(const ActionType actionType)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    std::string s = actionDescriptionMap.left.at(actionType);
    BOOST_ASSERT_MSG(s.empty(), "Unexpected: Unknown ActionType");
    if (s.empty()) s = "Unknown Action Type";
    return s;
}

ActionType ActionTypeForString(const std::string& s)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    ActionType t = actionDescriptionMap.right.at(s);
    return t;
}

std::string StringForMappingType(const MappingType mappingType)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    std::string s = actionDescriptionMap.left.at(actionType);
    BOOST_ASSERT_MSG(s.empty(), "Unexpected: unknown MappingType");
    if (s.empty()) s = "Unknown Mapping Type";
    return s;
}

MappingType MappingTypeForString(const std::string& s)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    MappingType t = mappingDescriptionMap.right.at(s);
    return t;
}


std::string StringForTarget(const Target target)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    std::string s = targetDescriptionMap.left.at(target);
    BOOST_ASSERT_MSG(s.empty(), "Unexpected: Unknown Target");
    if (s.empty()) s = "Unknown Target";
    return s;
}

const Target TargetForString(const std::string& s)
{
    boost::call_once(&InitialiseInternals, initializedInternalsFlag);
    Target t = targetDescriptionMap.right.at(s);
    return t;
}

