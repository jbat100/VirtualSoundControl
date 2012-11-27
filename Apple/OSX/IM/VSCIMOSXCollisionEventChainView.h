//
//  VSCIMOSXCollisionEventChainView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/29/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCOSXOBSceneElementEditor.h"
#import "VSCOSXOBSceneElementController.h"
#import "VSCIMOSXCollisionEventChainEditor.h"

#include "VSCIMEvent.h"

@class PXListView;
@class VSCIMOSXCollisionMappingListView;

/*
 *  Container view for the event chain view and the controls to edit 
 *  add and remove events
 */

@interface VSCIMOSXCollisionEventChainView : NSView <VSCIMOSXCollisionEventChainEditor>

@property (weak) id<VSCOSXOBSceneElementController> elementController;

@property (nonatomic, assign) VSC::IM::CollisionEventChain::WPtr eventChain;

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

-(VSC::IM::Event::SPtr) selectedChainEvent;

/*
 *  Mapping list view
 */

@property (strong)  VSCIMOSXCollisionMappingListView* mappingListView;

/*
 *  
 */

-(void) reloadInterface;

@end
