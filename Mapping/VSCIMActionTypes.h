
#include "VSCIMAction.h"

#include <string>

namespace VSC
{
    namespace IM
    {
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
        
        ActionType actionTypeForAction(Action::SPtr action);
        
        /*
         *  Use by controllers (VSCIMOSXActionController protocol) to create actions based on type
         */
        
        Action::SPtr createActionWithType(ActionType actionType);
        
        /*
         *  Used for UI/Debug display
         */
        
        std::string stringForActionType(ActionType actionType);
    }
}


