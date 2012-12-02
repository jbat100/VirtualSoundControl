//
//  VSCIMOSXCollisionEventChainViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "PXListViewDelegate.h"
#import "VSCOSXOBSceneElementEditor.h"
#import "VSCOSXOBSceneElementController.h"
#import "VSCIMOSXCollisionEventChainController.h"

#include "VSCIMEvent.h"

@class PXListView;
@class VSCIMOSXCollisionActionMappingsViewController;

@interface VSCIMOSXCollisionEventChainViewController : NSViewController <VSCIMOSXCollisionEventChainController, PXListViewDelegate>

/*
 *  Event chain interface
 */

@property (weak) IBOutlet PXListView* eventListView;
@property (weak) IBOutlet NSPopUpButton* addEventButton;
@property (weak) IBOutlet NSButton* removeEventButton;
@property (weak) IBOutlet NSMenu* addEventMenu;
@property (weak) IBOutlet NSMenuItem* addDelayMenuItem;
@property (weak) IBOutlet NSMenuItem* addCollisionMIDINoteOnMenuItem;
@property (weak) IBOutlet NSMenuItem* addCollisionMIDINoteOffMenuItem;
@property (weak) IBOutlet NSMenuItem* addCollisionMIDINoteOnAndOffMenuItem;
@property (weak) IBOutlet NSMenuItem* addCollisionMIDIControlChangeMenuItem;

-(IBAction) removeSelectedEvent:(id)sender;
-(IBAction) menuItemAction:(id)sender;
-(IBAction) listViewTest:(id)sender;

-(VSC::IM::Event::SPtr) selectedChainEvent;

/*
 *  Mapping list view
 */

@property (strong)  VSCIMOSXCollisionActionMappingsViewController* actionMappingsViewController;

-(void) reloadInterface;

@end
