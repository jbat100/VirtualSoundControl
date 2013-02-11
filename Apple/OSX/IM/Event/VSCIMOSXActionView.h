//
//  VSCIMActionView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXEventView.h"

#include "VSCIM.h"

/*
 *  The actual NSView subclass which displays the collision action parameters and allows 
 *  the type of action to be switched.
 */

@interface VSCIMOSXActionView : VSCIMOSXEventView

+(CGFloat) heightOfViewForAction:(VSC::IM::Action_SPtr)action;

@property (nonatomic, strong) IBOutlet NSView* mainView;
@property (nonatomic, strong) IBOutlet NSTextField* actionTypeTextField;
@property (nonatomic, strong) IBOutlet NSButton* mappingsButton;

@property (nonatomic, strong) IBOutlet NSView* midiOutputView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiOutputPopUpButton;

@property (nonatomic, strong) IBOutlet NSView* midiChannelView;
@property (nonatomic, strong) IBOutlet NSTextField* midiChannelTextField;

@property (nonatomic, strong) IBOutlet NSView* midiControlNumberView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiControlNumberPopUpButton;

-(VSC::IM::Action_SPtr) action; // return cast event from VSCIMOSXEventView

-(IBAction) midiOutputSelected:(id)sender;
-(IBAction) midiControlNumberSelected:(id)sender;

-(IBAction) refreshMIDIOutputs:(id)sender;
-(IBAction) refreshMIDIControlNumbers:(id)sender;

/*
 *  Debugging
 */

-(void) printUIDescription;


@end
