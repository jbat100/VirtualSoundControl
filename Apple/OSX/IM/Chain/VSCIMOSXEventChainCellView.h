//
//  VSCIMOSXEventChainCellView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/30/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIMEventChain.h"

@interface VSCIMOSXEventChainCellView : NSView

@property (assign) id<VSCOSXEnvironmentController> environmentController;

@property (nonatomic, assign) VSC::IM::EventChain::WPtr eventChain;

@property (nonatomic, strong) IBOutlet NSTextField* labelTextField;
@property (nonatomic, strong) IBOutlet NSButton* editButton;

-(IBAction) editButtonClicked:(id)sender;

@end
