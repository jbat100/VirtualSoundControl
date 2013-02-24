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

@class VSCIMOSXEventListView;
@class VSCIMOSXEventEditorViewController;


@interface VSCIMOSXEventChainViewController : NSViewController
<VSCIMOSXEventChainController, NSTableViewDataSource, NSTableViewDelegate>

@property (assign) id<VSCOSXEnvironmentController> environmentController;

/*
 *  Event list view
 */

@property (nonatomic, strong) IBOutlet VSCIMOSXEventListView* eventListView;

-(IBAction) removeSelectedEvent:(id)sender;
-(IBAction) menuItemAction:(id)sender;
-(IBAction) listViewTest:(id)sender;

-(VSC::IM::Event_SPtr) selectedChainEvent;

/*
 *  Mapping list view
 */

@property (nonatomic, strong) VSCIMOSXEventEditorViewController* eventEditorViewController;

-(void) reloadInterface;

/*
 *  Debug
 */

-(void) printUIDescription;

@end
