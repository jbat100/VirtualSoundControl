//
//  VSCIMActionView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCIMOSXActionGlobal.h"
#import "VSCIMOSXMappingTypes.h"
#import "VSCIMOSXMappingView.h"

#include "VSCIMAction.h"

@protocol VSCIMOSXCollisionEventChainController;

/*
 *  The actual NSView subclass which displays the collision action parameters and allows 
 *  the type of action to be switched.
 */

@interface VSCIMOSXActionView : NSView

+(CGFloat) heightOfViewForCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction;
+(NSString*) stringForActionType:(VSCIMOSXActionType)actionType;
+(VSCIMOSXActionType) actionTypeForString:(NSString*)menuItemString;

@property (assign) IBOutlet id<VSCIMOSXCollisionEventChainController> eventChainController;

@property (nonatomic, assign) VSC::IM::CollisionAction::WPtr collisionAction;


@property (nonatomic, strong) IBOutlet NSView* mainView;
@property (nonatomic, strong) IBOutlet NSTextField* actionTypeTextField;
@property (nonatomic, strong) IBOutlet NSButton* mappingsButton;

@property (nonatomic, strong) IBOutlet NSView* midiSetupView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiOutputPopUpButton;
@property (nonatomic, strong) IBOutlet NSTextField* midiChannelTextField;

@property (nonatomic, strong) IBOutlet NSView* midiControlSetupView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiControlNumberPopUpButton;

-(IBAction) showMappings:(id)sender;
-(IBAction) midiOutputSelected:(id)sender;
-(IBAction) midiControlNumberSelected:(id)sender;

-(IBAction) refreshMIDIOutputs:(id)sender;
-(IBAction) refreshMIDIControlNumbers:(id)sender;

/*
 *  Debugging
 */

-(void) printUIDescription;


@end
