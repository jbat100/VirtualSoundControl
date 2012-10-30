//
//  VSCIMOSXCollisionEventChainView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/29/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "PXListView.h"
#import "VSCIMOSXCollisionActionView.h"

#include "VSCIMEvent.h"
#include "VSCIMCollisionEventChain.h"
#include "VSCIMCollisionAction.h"

@interface VSCIMOSXCollisionEventChainView : NSView <PXListViewDelegate, VSCIMOSXCollisionActionViewDelegate>

@property (nonatomic, strong) IBOutlet PXListView* collisionEventListView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* addEventButton;
@property (nonatomic, strong) IBOutlet NSButton* removeEventButton;
@property (nonatomic, strong) IBOutlet NSMenu* addEventMenu;

@property (nonatomic, assign) VSC::IM::CollisionEventChain::WPtr collisionEventChain;

-(VSC::IM::Event::SPtr) selectedEvent;

/*
 *  UI Callbacks
 */

-(IBAction) addEvent:(id)sender;
-(IBAction) removeSelectedEvent:(id)sender;

/*
 *  Collision Actions called directly from menu items
 */

-(IBAction) prependNewCollisionAction:(id)sender;
-(IBAction) appendNewCollisionAction:(id)sender;
-(IBAction) insertNewCollisionActionBeforeSelection:(id)sender;
-(IBAction) insertNewCollisionActionAfterSelection:(id)sender;

/*
 *  Delays called directly from menu items
 */

-(IBAction) prependNewDelay:(id)sender;
-(IBAction) appendNewDelay:(id)sender;
-(IBAction) insertNewDelayBeforeSelection:(id)sender;
-(IBAction) insertNewDelayAfterSelection:(id)sender;

@end
