//
//  VSCIMOSXEventChainListView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/30/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIM.h"

@protocol VSCOSXEnvironmentController;

@interface VSCIMOSXEventChainListView : NSView

@property (assign) IBOutlet id<VSCOSXEnvironmentController> environmentController;

@property (nonatomic, strong) IBOutlet NSButton* addButton;
@property (nonatomic, strong) IBOutlet NSButton* removeButton;

@property (nonatomic, strong) IBOutlet NSTableView* tableView;

-(void) reloadInterface;

-(VSC::IM::EventChain_SPtr) selectedEventChain;

@end
