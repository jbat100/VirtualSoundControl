//
//  VSCIMOSXCollisionActionMappingsViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXCollisionMappingTypes.h"
#import "PXListViewDelegate.h"

#include "VSCIMCollisionAction.h"

@class PXListView;
@class VSCIMOSXCollisionMappingEditViewController;
@protocol VSCIMOSXCollisionEventChainController;


@interface VSCIMOSXCollisionActionMappingsViewController : NSViewController

@property (weak) id<VSCIMOSXCollisionEventChainController> eventChainController;

@property (nonatomic, assign) VSC::IM::CollisionAction::WPtr action;

@property (weak) IBOutlet PXListView* listView;

@end
