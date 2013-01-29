//
//  VSCIMOSXActionMappingsViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXMappingTypes.h"
#import "VSCIMOSXActionMappingsController.h"

#include "VSCIMAction.h"

@class VSCIMOSXMappingEditViewController;
@protocol VSCIMOSXEventChainController;


@interface VSCIMOSXActionMappingsViewController : NSViewController
<VSCIMOSXActionMappingsController, NSTableViewDelegate, NSTableViewDataSource>

@property (weak) id<VSCIMOSXEventChainController> eventChainController;
@property (nonatomic, weak) IBOutlet NSTableView* mappingTableView;
@property (nonatomic, strong) VSCIMOSXMappingEditViewController* collisionMappingEditViewController;
@property (nonatomic, strong) NSPopover* collisionMappingEditPopover;

-(void) reloadInterface;

-(IBAction) backToEventChainView:(id)sender;

@end
