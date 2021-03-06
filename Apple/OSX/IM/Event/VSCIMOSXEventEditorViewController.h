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

/*
 *  The editingView will contain everything apart from the back button
 *  including the mappingTableScrollView (which contains mappingTableView) 
 */

@property (nonatomic, weak) IBOutlet NSView* editingView;

/*
 *  Mapping stuff
 */

@property (nonatomic, weak) IBOutlet NSScrollView* mappingTableScrollView;
@property (nonatomic, weak) IBOutlet NSTableView* mappingTableView;

@property (nonatomic, strong) VSCIMOSXMappingEditViewController* mappingEditViewController;
@property (nonatomic, strong) NSPopover* mappingEditPopover;

-(void) reloadInterface;

-(IBAction) backToEventChainView:(id)sender;

@end
