//
//  VSCIMOSXCollisionActionMappingsViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXCollisionMappingTypes.h"
#import "VSCIMOSXCollisionActionMappingsController.h"

#include "VSCIMCollisionAction.h"

@class VSCIMOSXCollisionMappingEditViewController;
@protocol VSCIMOSXCollisionEventChainController;


@interface VSCIMOSXCollisionActionMappingsViewController : NSViewController
<VSCIMOSXCollisionActionMappingsController, NSTableViewDelegate, NSTableViewDataSource>

@property (weak) id<VSCIMOSXCollisionEventChainController> eventChainController;
@property (nonatomic, weak) IBOutlet NSTableView* mappingTableView;
@property (nonatomic, strong) VSCIMOSXCollisionMappingEditViewController* collisionMappingEditViewController;
@property (nonatomic, strong) NSPopover* collisionMappingEditPopover;

-(void) reloadInterface;

-(IBAction) backToEventChainView:(id)sender;

@end
