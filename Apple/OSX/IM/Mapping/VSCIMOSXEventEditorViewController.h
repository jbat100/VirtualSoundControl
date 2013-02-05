//
//  VSCIMOSXEventEditorViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXEventEditor.h"

#include "VSCIM.h"

@class VSCIMOSXMappingEditViewController;
@protocol VSCIMOSXEventChainController;


@interface VSCIMOSXEventEditorViewController : NSViewController
<VSCIMOSXEventEditor, NSTableViewDelegate, NSTableViewDataSource>

@property (weak) id<VSCIMOSXEventChainController> eventChainController;

@property (nonatomic, strong) VSCIMOSXMappingEditViewController* collisionMappingEditViewController;
@property (nonatomic, strong) NSPopover* collisionMappingEditPopover;

@property (nonatomic, weak) IBOutlet NSTableView* mappingTableView;
@property (nonatomic, weak) IBOutlet NSTableView* collisionMappingTableView;

-(void) reloadInterface;

-(IBAction) backToEventChainView:(id)sender;

@end
