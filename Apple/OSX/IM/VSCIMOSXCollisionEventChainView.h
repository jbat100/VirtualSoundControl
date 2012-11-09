//
//  VSCIMOSXCollisionEventChainView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/29/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIMEvent.h"

@class PXListView;
@protocol VSCIMOSXCollisionEventChainController;

/*
 *  Container view for the event chain view and the controls to edit 
 *  add and remove events
 */

@interface VSCIMOSXCollisionEventChainView : NSView

@property (weak) id<VSCIMOSXCollisionEventChainController> eventChainController;

@property (nonatomic, strong) IBOutlet PXListView* eventListView;

@property (nonatomic, strong) IBOutlet NSPopUpButton* addEventButton;
@property (nonatomic, strong) IBOutlet NSButton* removeEventButton;
@property (nonatomic, strong) IBOutlet NSMenu* addEventMenu;

-(VSC::IM::Event::SPtr) selectedChainEvent;

@end
