//
//  VSCIMCollisionActionView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "PXListViewCell.h"

#import "VSCIMOSXCollisionActionGlobal.h"
#import "VSCIMOSXCollisionMappingGlobal.h"
#import "VSCIMOSXCollisionMappingView.h"

#include "VSCIMCollisionAction.h"

@protocol VSCIMOSXCollisionEventChainController;

/*
 *  The actual NSView subclass which displays the collision action parameters and allows 
 *  the type of action to be switched.
 */

@interface VSCIMOSXCollisionActionView : PXListViewCell

+(CGFloat) heightOfViewForCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction;

+(NSString*) stringForActionType:(VSCIMOSXCollisionActionType)actionType;
+(VSCIMOSXCollisionActionType) actionTypeForString:(NSString*)menuItemString;

@property (weak) id<VSCIMOSXCollisionEventChainController> eventChainController;

@property (nonatomic, assign) VSC::IM::CollisionAction::WPtr collisionAction;

@end
