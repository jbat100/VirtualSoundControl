//
//  VSCIMOSXEventChainViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCOBOSXElementEditor.h"
#import "VSCOBOSXElementController.h"
#import "VSCIMOSXEventChainController.h"

#include "VSCIM.h"

@class VSCIMOSXEventEditorViewController;

@interface VSCIMOSXEventChainViewController : NSViewController
<VSCIMOSXEventChainController, NSTableViewDataSource, NSTableViewDelegate>

/*
 *  Event chain interface
 */

@property (weak) IBOutlet NSTableView* eventTableView;
@property (weak) IBOutlet NSPopUpButton* addEventButton;
@property (weak) IBOutlet NSButton* removeEventButton;
@property (weak) IBOutlet NSMenu* addEventMenu;

@property (weak) IBOutlet NSMenuItem* addDelayMenuItem;
@property (weak) IBOutlet NSMenuItem* addActionMenuItem;

-(IBAction) removeSelectedEvent:(id)sender;
-(IBAction) menuItemAction:(id)sender;
-(IBAction) listViewTest:(id)sender;

-(VSC::IM::Event_SPtr) selectedChainEvent;

/*
 *  Mapping list view
 */

@property (strong)  VSCIMOSXEventEditorViewController* eventEditorViewController;

-(void) reloadInterface;

/*
 *  Debug
 */

-(void) printUIDescription;

@end
