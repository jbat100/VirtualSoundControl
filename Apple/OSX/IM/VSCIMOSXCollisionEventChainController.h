//
//  VSCIMOSXCollisionEventChainController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/29/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PXListViewDelegate.h"

#include "VSCIMCollisionEventChain.h"
#include "VSCIMEvent.h"

/*
 *  Using a protocol to decrease coupling
 */

@protocol VSCIMOSXCollisionEventChainController <NSObject, PXListViewDelegate>

@property (nonatomic, assign) VSC::IM::CollisionEventChain::WPtr collisionEventChain;

-(void) showCollisionEventChain;

-(void) showCollisionMappingsForCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction;

@end

/*
 *  A concrete VSCIMOSXCollisionEventChainController
 */

@class VSCIMOSXCollisionEventChainView;
@class VSCIMOSXCollisionMappingListView;

@class VSCIMOSXCollisionMappingView;
@class VSCIMOSXCollisionActionView;
@class VSCIMOSXDelayView;

@interface VSCIMOSXCollisionEventChainController : NSObject <VSCIMOSXCollisionEventChainController>

@property (strong) NSView* mainView;

@property (strong) IBOutlet VSCIMOSXCollisionEventChainView* collisionEventChainView;
@property (strong) IBOutlet VSCIMOSXCollisionMappingListView* collisionMappingListView;

-(VSCIMOSXCollisionMappingView*) newCollisionMappingView;
-(VSCIMOSXCollisionActionView*) newCollisionActionView;
-(VSCIMOSXDelayView*) newDelayView;


@end
