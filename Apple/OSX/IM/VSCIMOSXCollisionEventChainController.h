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

-(void) showCollisionMappingsForCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction;

@end

/*
 *  A concrete VSCIMOSXCollisionEventChainController
 */

@class VSCIMOSXCollisionEventChainView;
@class VSCIMOSXCollisionMappingView;

@interface VSCIMOSXCollisionEventChainController : NSObject <VSCIMOSXCollisionEventChainController>

@property (nonatomic, strong) IBOutlet VSCIMOSXCollisionEventChainView* collisionEventChainView;
@property (nonatomic, strong) NSNib* mappingViewNib;


-(VSCIMOSXCollisionMappingView*) newCollisionMappingView;


@end
