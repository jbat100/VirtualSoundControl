
#ifndef _VSC_IM_H_
#define _VSC_IM_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "VSCOB.h"

#include <vector>
#include <set>

namespace VSC
{
    namespace IM
    {
        /**
         *  Forward class declarations
         */
        
        class Event;
        typedef boost::shared_ptr<Event>        Event_SPtr;
        typedef boost::weak_ptr<Event>          Event_WPtr;
        typedef std::vector<Event_SPtr>         Events;
        
        class EventChain;
        typedef boost::shared_ptr<EventChain>   EventChain_SPtr;
        typedef boost::weak_ptr<EventChain>     EventChain_WPtr;
        typedef std::vector<EventChain_SPtr>    EventChains;
        
        class Action;
        typedef boost::shared_ptr<Action>       Action_SPtr;
        typedef boost::weak_ptr<Action>         Action_WPtr;
        
        class Delay;
        typedef boost::shared_ptr<Delay>        Delay_SPtr;
        typedef boost::weak_ptr<Delay>          Delay_WPtr;
        
        class Mapping;
        typedef boost::shared_ptr<Mapping>      Mapping_SPtr;
        typedef boost::weak_ptr<Mapping>        Mapping_WPtr;
        typedef std::vector<Mapping_SPtr>       Mappings;
        
        /*
         *  Action types enum, used to avoid having to downcast constantly to check action type.
         *  Not sure whether this is the best way of doing things. Should collision actions be polymorphic
         *  in the first place. If they weren't all task creations would have to be defined in the same
         *  place. No extention possible, which is a serious restriction.
         */
        
        enum ActionType
        {
            ActionTypeNone = 0,
            ActionTypeMIDINoteOn,
            ActionTypeMIDINoteOnAndOff,
            ActionTypeMIDINoteOff,
            ActionTypeMIDIControlChange
        };
        
        /*
         *  Used for UI/Debug display
         */
        
        std::string StringForActionType(const ActionType actionType);
        
        ActionType ActionTypeForString(const std::string& s);
        
        /*
         *  Mapping type enum
         */
        
        enum MappingType
        {
            MappingTypeNone = 0,
            MappingTypeConstant,
            MappingTypeCollisionVelocity,
            MappingTypeCollisionDistance
        };
        
        typedef std::set<MappingType> MappingTypeSet;
        
        std::string StringForMappingType(const MappingType mappingType);
        
        MappingType MappingTypeForString(const std::string& s);
        
        
        /**
         *  Targets
         */
        
        enum Target {
            TargetNone = 0,
            TargetChannel,
            TargetPitch,
            TargetVelocityOn,
            TargetVelocityOff,
            TargetDuration,
            TargetValue
        };
        
        typedef std::vector<Target> Targets;
        
        std::string StringForTarget(const Target target);
        
        const Target TargetForString(const std::string& s);
        
        /**
         *  Triggers
         */
        
        enum Trigger {
            TriggerNone = 0,
            TriggerPlain,
            TriggerCollision,
            TriggerProximity
        };
        
        typedef std::vector<Trigger> Triggers;
        
        std::string StringForTarget(const Target target);
        
        const Target TargetForString(const std::string& s);
        
        const MappingTypeSet& AllowedMappingTypeSetForTrigger(Trigger trigger);
        
        const Triggers& AllowedTriggers(void);
        
        /**
         *  Trigger payloads
         */
        
        struct TriggerPayload
        {
            typedef boost::shared_ptr<TriggerPayload>   SPtr;
            typedef boost::weak_ptr<TriggerPayload>     WPtr;
        };
        
        struct TriggerPlainPayload : public TriggerPayload
        {
            typedef boost::shared_ptr<TriggerPayload>   SPtr;
            // Nothing special yet but will surely think of something later...
        };
        
        struct TriggerCollisionPayload : public TriggerPayload
        {
            typedef boost::shared_ptr<TriggerPayload>   SPtr;
            OB::Collision_SPtr  collision;
            OB::Element_SPtr    effector;
        };
        
        struct TriggerProximityPayload : public TriggerPayload
        {
            typedef boost::shared_ptr<TriggerPayload>   SPtr;
            OB::Proximity_SPtr  proximity;
        };
    }
}

#endif // _VSC_IM_H_
