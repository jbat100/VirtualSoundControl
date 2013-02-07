
#include "VSCIM.h"
#include "VSCIMAction.h"
#include "VSCIMActionImplementations.h"
#include "VSCIMCollisionMapping.h"

#include <boost/assign/std/set.hpp>
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
        
        TargetDescriptionMap    targetDescriptionMap;
        MappingDescriptionMap   mappingDescriptionMap;
        ActionDescriptionMap    actionDescriptionMap;
        
        typedef std::map<Trigger, MappingTypeSet> TriggerMappingTypeMap;
        
        TriggerMappingTypeMap   triggerMappingTypeMap;
    }
}

using namespace VSC;
using namespace VSC::IM;

boost::once_flag initialzedMapsFlag = BOOST_ONCE_INIT;

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
    
    MappingTypeSet plainSet;
    plainSet += MappingTypeConstant;
    triggerMappingTypeMap[TriggerPlain] = plainSet;
    
    MappingTypeSet collisionSet;
    collisionSet += MappingTypeConstant, MappingTypeCollisionVelocity, MappingTypeCollisionDistance;
    triggerMappingTypeMap[TriggerCollision] = collisionSet;
    
    MappingTypeSet proximitySet;
    proximitySet += MappingTypeConstant;
    triggerMappingTypeMap[TriggerProximity] = proximitySet;
}

const MappingTypeSet& allowedMappingTypeSetForTrigger(Trigger trigger)
{
    boost::call_once(&InitialiseMaps, initialzedMapsFlag);
    return triggerMappingTypeMap[trigger];
}

std::string stringForActionType(const ActionType actionType)
{
    boost::call_once(&InitialiseMaps, initialzedMapsFlag);
    std::string s = actionDescriptionMap.left.at(actionType);
    BOOST_ASSERT_MSG(s.empty(), "Unexpected: Unknown ActionType");
    if (s.empty()) s = "Unknown Action Type";
    return s;
}

ActionType actionTypeForString(const std::string& s)
{
    boost::call_once(&InitialiseMaps, initialzedMapsFlag);
    ActionType t = actionDescriptionMap.right.at(s);
    return t;
}

std::string stringForMappingType(const MappingType mappingType)
{
    boost::call_once(&InitialiseMaps, initialzedMapsFlag);
    std::string s = actionDescriptionMap.left.at(actionType);
    BOOST_ASSERT_MSG(s.empty(), "Unexpected: unknown MappingType");
    if (s.empty()) s = "Unknown Mapping Type";
    return s;
}

MappingType mappingTypeForString(const std::string& s)
{
    boost::call_once(&InitialiseMaps, initialzedMapsFlag);
    MappingType t = mappingDescriptionMap.right.at(s);
    return t;
}


std::string stringForTarget(const Target target)
{
    boost::call_once(&InitialiseMaps, initialzedMapsFlag);
    std::string s = targetDescriptionMap.left.at(target);
    BOOST_ASSERT_MSG(s.empty(), "Unexpected: Unknown Target");
    if (s.empty()) s = "Unknown Target";
    return s;
}

const Target targetForString(const std::string& s)
{
    boost::call_once(&InitialiseMaps, initialzedMapsFlag);
    Target t = targetDescriptionMap.right.at(s);
    return t;
}

