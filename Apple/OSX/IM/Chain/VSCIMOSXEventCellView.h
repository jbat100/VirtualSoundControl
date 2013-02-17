//
//  VSCIMOSXEventCellView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 2/3/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIM.h"

@protocol VSCIMOSXEventChainController;

@interface VSCIMOSXEventCellView : NSView

@property (assign) IBOutlet id<VSCIMOSXEventChainController> eventChainController;

@property (nonatomic, assign) VSC::IM::Event_WPtr event;

@property (nonatomic, weak) IBOutlet NSImageView* iconImageView;
@property (nonatomic, weak) IBOutlet NSButton* editorButton;

+(CGFloat) defaultViewHeight;

-(IBAction) showEditor:(id)sender;

-(void) reloadInterface;
-(void) printUIDescription;

/**
 *  Gives subclasses the oportunity to refuse events if they are not the right subclass
 *  of VSC::IM::Event
 */

-(BOOL) checkEvent:(VSC::IM::Event_SPtr)testEvent;

@end
