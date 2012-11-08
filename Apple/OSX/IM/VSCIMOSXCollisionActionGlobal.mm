//
//  VSCIMOSXCollisionActionGlobal.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionActionGlobal.h"

#include "VSCIMCollisionMIDIActions.h"

VSCIMOSXCollisionActionType VSCIMOSXCollisionActionTypeForCollisionAction(VSC::IM::CollisionAction::SPtr action)
{
    if (action)
    {
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionVoidAction>(action)) return VSCIMOSXCollisionActionTypeVoid;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionMIDINoteOnAction>(action)) return VSCIMOSXCollisionActionTypeMIDINoteOn;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionMIDINoteOnAndOffAction>(action)) return VSCIMOSXCollisionActionTypeMIDINoteOnAndOff;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionMIDINoteOffAction>(action)) return VSCIMOSXCollisionActionTypeMIDINoteOff;
        
        if (boost::dynamic_pointer_cast<VSC::IM::CollisionMIDIControlChangeAction>(action)) return VSCIMOSXCollisionActionTypeMIDIControlChange;
    }
    
    return VSCIMOSXCollisionActionTypeNone;
}

VSC::IM::CollisionAction::SPtr VSCIMOSXCreateCollisionActionWithType(VSCIMOSXCollisionActionType actionType)
{
    switch (actionType) {
        
        case VSCIMOSXCollisionActionTypeMIDINoteOn:
            return VSC::IM::CollisionAction::SPtr(new VSC::IM::CollisionMIDINoteOnAction);
            break;
            
        case VSCIMOSXCollisionActionTypeMIDINoteOnAndOff:
            return VSC::IM::CollisionAction::SPtr(new VSC::IM::CollisionMIDINoteOnAndOffAction);
            break;
            
        case VSCIMOSXCollisionActionTypeMIDINoteOff:
            return VSC::IM::CollisionAction::SPtr(new VSC::IM::CollisionMIDINoteOffAction);
            break;
            
        case VSCIMOSXCollisionActionTypeMIDIControlChange:
            return VSC::IM::CollisionAction::SPtr(new VSC::IM::CollisionMIDIControlChangeAction);
            break;
            
        default:
            break;
    }
    
    return VSC::IM::CollisionAction::SPtr();
}

