//
//  VSCIMCollisionActionView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXCollisionMappingView.h"

#include "VSCIMCollisionAction.h"

typedef enum _VSCIMOSXCollisionActionType {
    VSCIMOSXCollisionActionTypeNone = 0,
    VSCIMOSXCollisionActionTypeVoid,
    VSCIMOSXCollisionActionTypeMIDINoteOn,
    VSCIMOSXCollisionActionTypeMIDINoteOnAndOff,
    VSCIMOSXCollisionActionTypeMIDINoteOff,
    VSCIMOSXCollisionActionTypeMIDIControlChange
} VSCIMOSXCollisionActionType;

@class VSCIMOSXCollisionActionView;

/*
 *  Protocol used by the view to interact with the model, specifically to ask for a collision
 *  action type change.
 */

@protocol VSCIMOSXCollisionActionViewDelegate

/*
 *  Use when a new type (subclass) of collision action has been created by the view
 *  to inform the delegate of the change in pointer.
 */

-(VSC::IM::CollisionAction::SPtr) collisionActionView:(VSCIMOSXCollisionActionView*)view
                      requestsCollisionActionWithType:(VSCIMOSXCollisionActionType)collisionActionType;


@end

/*
 *  The actual NSView subclass which displays the collision action parameters and allows 
 *  the type of action to be switched.
 */

@interface VSCIMOSXCollisionActionView : NSView <VSCIMOSXCollisionMappingViewDelegate>

+(CGFloat) heightOfViewForCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction;

@property (weak) id<VSCIMOSXCollisionActionViewDelegate> delegate;

@property (nonatomic, assign) VSC::IM::CollisionAction::SPtr collisionAction;
@property (nonatomic, strong) IBOutlet NSPopUpButton* actionTypePopUpButton;

@end
