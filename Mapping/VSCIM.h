
#ifndef _VSC_IM_H_
#define _VSC_IM_H_

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

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
         *  Performs dynamic casts until it gets the type, very slow, use as little as possible
         *  (Editors should cache the type internally and update it when their associated action
         *  is set)
         */
        
        ActionType actionTypeForAction(Action_SPtr action);
        
        /*
         *  Use by controllers (VSCIMOSXActionController protocol) to create actions based on type
         */
        
        Action_SPtr createActionWithType(ActionType actionType);
        
        /*
         *  Used for UI/Debug display
         */
        
        std::string stringForActionType(ActionType actionType);
        
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
        
        MappingType mappingTypeForMapping(Mapping_SPtr mapping);
        
        Mapping_SPtr createMappingWithType(MappingType mappingType);
        
        std::string stringForMappingType(MappingType mappingType);
    }
}

#endif // _VSC_IM_H_
