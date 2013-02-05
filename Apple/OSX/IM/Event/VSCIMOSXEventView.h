//
//  VSCIMOSXEventView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 2/3/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIM.h"

@protocol VSCIMOSXEventChainController;

@interface VSCIMOSXEventView : NSView

@property (assign) IBOutlet id<VSCIMOSXEventChainController> eventChainController;

@property (nonatomic, assign) VSC::IM::Event_WPtr event;

-(IBAction) showMappings:(id)sender;

-(void) reloadInterface;

/**
 *  Gives subclasses the oportunity to refuse events if they are not the right subclass
 *  of VSC::IM::Event
 */

-(BOOL) checkEvent:(VSC::IM::Event_SPtr)testEvent;

@end
