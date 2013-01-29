//
//  VSCIMOSXActionGlobal.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXActionGlobal.h"

#include "VSCIMCollisionMIDIActions.h"

VSCIMOSXActionType VSCIMOSXActionTypeForCollisionAction(VSC::IM::CollisionAction::SPtr action)
{
    if (action)
    {
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionVoidAction>(action))
            return VSCIMOSXActionTypeVoid;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionMIDINoteOnAction>(action))
            return VSCIMOSXActionTypeMIDINoteOn;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionMIDINoteOnAndOffAction>(action))
            return VSCIMOSXActionTypeMIDINoteOnAndOff;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionMIDINoteOffAction>(action))
            return VSCIMOSXActionTypeMIDINoteOff;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionMIDIControlChangeAction>(action))
            return VSCIMOSXActionTypeMIDIControlChange;
    }
    
    return VSCIMOSXActionTypeNone;
}

VSC::IM::CollisionAction::SPtr VSCIMOSXCreateCollisionActionWithType(VSCIMOSXActionType actionType)
{
    switch (actionType) {
        
        case VSCIMOSXActionTypeMIDINoteOn:
            return VSC::IM::CollisionAction::SPtr(new VSC::IM::CollisionMIDINoteOnAction);
            break;
            
        case VSCIMOSXActionTypeMIDINoteOnAndOff:
            return VSC::IM::CollisionAction::SPtr(new VSC::IM::CollisionMIDINoteOnAndOffAction);
            break;
            
        case VSCIMOSXActionTypeMIDINoteOff:
            return VSC::IM::CollisionAction::SPtr(new VSC::IM::CollisionMIDINoteOffAction);
            break;
            
        case VSCIMOSXActionTypeMIDIControlChange:
            return VSC::IM::CollisionAction::SPtr(new VSC::IM::CollisionMIDIControlChangeAction);
            break;
            
        default:
            break;
    }
    
    return VSC::IM::CollisionAction::SPtr();
}

