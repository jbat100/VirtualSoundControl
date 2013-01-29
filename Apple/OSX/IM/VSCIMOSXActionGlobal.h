//
//  VSCIMOSXActionGlobal.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCIMAction.h"

/*
 *  Action types enum, used to avoid having to downcast constantly to check action type.
 *  Not sure whether this is the best way of doing things. Should collision actions be polymorphic 
 *  in the first place. If they weren't all task creations would have to be defined in the same 
 *  place. No extention possible, which is a serious restriction.
 */

typedef enum _VSCIMOSXActionType {
    VSCIMOSXActionTypeNone = 0,
    VSCIMOSXActionTypeVoid,
    VSCIMOSXActionTypeMIDINoteOn,
    VSCIMOSXActionTypeMIDINoteOnAndOff,
    VSCIMOSXActionTypeMIDINoteOff,
    VSCIMOSXActionTypeMIDIControlChange
} VSCIMOSXActionType;

/*
 *  Performs dynamic casts until it gets the type, very slow, use as little as possible
 *  (Editors should cache the type internally and update it when their associated action
 *  is set)
 */

VSCIMOSXActionType VSCIMOSXActionTypeForCollisionAction(VSC::IM::CollisionAction::SPtr action);

/*
 *  Use by controllers (VSCIMOSXActionController protocol) to create actions based on type
 */

VSC::IM::CollisionAction::SPtr VSCIMOSXCreateCollisionActionWithType(VSCIMOSXActionType actionType);


