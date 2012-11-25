//
//  VSCOSXOBSceneElementCollisionEditView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/24/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIMCollisionEventChain.h"
#include "VSCIMCollisionAction.h"

@class VSCIMOSXCollisionEventChainView;
@class VSCIMOSXCollisionMappingListView;

@interface VSCOSXOBSceneElementCollisionEditView : NSView

@property (weak)    IBOutlet NSView* mainContainerView;

@property (strong)  VSCIMOSXCollisionEventChainView* eventChainView;
@property (strong)  VSCIMOSXCollisionMappingListView* mappingListView;

-(void) reloadInterface;
-(void) showEventChainView;
-(void) showMappingsForCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction;

@end
