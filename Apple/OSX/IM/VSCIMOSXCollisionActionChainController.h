//
//  VSCIMOSXCollisionActionChainController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/29/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "VSCIMOSXCollisionActionChainView.h"

#include "VSCIMCollisionActionChain.h"
#include "VSCIMCollisionAction.h"

@interface VSCIMOSXCollisionActionChainController : NSObject

@property (nonatomic, assign) VSC::IM:CollisionActionChain::WPtr collisionActionChain;

-(void) addCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction;
-(void) removeCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction;

@end
