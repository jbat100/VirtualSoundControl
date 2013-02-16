//
//  VSCIMOSXEventChainCellView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/30/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIMEventChain.h"

@protocol VSCOSXEnvironmentController;

@interface VSCIMOSXEventChainCellView : NSView

@property (assign) IBOutlet id<VSCOSXEnvironmentController> environmentController;

@property (nonatomic, assign) VSC::IM::EventChain::WPtr eventChain;

@property (nonatomic, strong) IBOutlet NSTextField* labelTextField;
@property (nonatomic, strong) IBOutlet NSButton* editButton;
@property (nonatomic, strong) IBOutlet NSImageView* imageView;

+(CGFloat) defaultViewHeight;

-(IBAction) editButtonClicked:(id)sender;

@end
