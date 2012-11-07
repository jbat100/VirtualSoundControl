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

@interface VSCIMOSXCollisionEventChainView : NSView

@property (nonatomic, strong) IBOutlet PXListView* collisionEventListView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* addEventButton;
@property (nonatomic, strong) IBOutlet NSButton* removeEventButton;
@property (nonatomic, strong) IBOutlet NSMenu* addEventMenu;

-(VSC::IM::Event::SPtr) selectedChainEvent;

@end
