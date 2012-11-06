//
//  VSCIMOSXCollisionActionGlobal.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCIMCollisionAction.h"

/*
 *  Action types enum, used to avoid having to downcast constantly to check action type.
 *  Not sure whether this is the best way of doing things. Should collision actions be polymorphic 
 *  in the first place. If they weren't all task creations would have to be defined in the same 
 *  place. No extention possible, which is a serious restriction.
 */

typedef enum _VSCIMOSXCollisionActionType {
    VSCIMOSXCollisionActionTypeNone = 0,
    VSCIMOSXCollisionActionTypeVoid,
    VSCIMOSXCollisionActionTypeMIDINoteOn,
    VSCIMOSXCollisionActionTypeMIDINoteOnAndOff,
    VSCIMOSXCollisionActionTypeMIDINoteOff,
    VSCIMOSXCollisionActionTypeMIDIControlChange
} VSCIMOSXCollisionActionType;

/*
 *  Performs dynamic casts until it gets the type, very slow, use as little as possible
 *  (Editors should cache the type internally and update it when their associated action
 *  is set)
 */

VSCIMOSXCollisionActionType VSCIMOSXCollisionActionTypeForCollisionAction(VSC::IM::CollisionAction::SPtr action);

/*
 *  Use by controllers (VSCIMOSXCollisionActionController protocol) to create actions based on type
 */

VSC::IM::CollisionAction::SPtr VSCIMOSXCreateCollisionActionWithType(VSCIMOSXCollisionActionType actionType);

/*
 *  Check if action is midi action, useful for interface (editor needs channel text field)
 */

BOOL VSCIMOSXCollisionActionTypeIsMIDI(VSCIMOSXCollisionActionType actionType);

/*
 *  Forward declaration needed for VSCIMOSXCollisionActionController protocol
 */

@protocol VSCIMOSXCollisionActionEditor;

/*
 *  Protocol used by the view to interact with the model, specifically to ask for a collision
 *  action type change.
 */

@protocol VSCIMOSXCollisionActionController <NSObject>

/*
 *  Use when a new type (subclass) of collision action has been created by the view
 *  to inform the delegate of the change in pointer.
 */

-(VSC::IM::CollisionAction::SPtr) collisionActionEditor:(id<VSCIMOSXCollisionActionEditor>)editor
                        requestsCollisionActionWithType:(VSCIMOSXCollisionActionType)collisionActionType;

@end

/*
 *  Protocol used for editor viewss for example
 */

@protocol VSCIMOSXCollisionActionEditor <NSObject>

@property (weak) id<VSCIMOSXCollisionActionController> controller;

@property (nonatomic, assign) VSC::IM::CollisionAction::WPtr collisionAction;

@end
