//
//  VSCIMOSXCollisionActionGlobal.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCIMCollisionAction.h"

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
