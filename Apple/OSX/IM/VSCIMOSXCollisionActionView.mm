//
//  VSCIMCollisionActionView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionActionView.h"

#include <boost/assert.hpp>

NSDictionary* actionTypeMenuItemStringDict = nil;

/*
 *  Private internals
 */

@interface VSCIMOSXCollisionActionView ()

@property (nonatomic, assign) VSCIMOSXCollisionActionType currentActionType;

@property (nonatomic, strong) IBOutlet NSPopUpButton* actionTypePopUpButton;

+(NSString*) stringForActionTypeMenuItem:(VSCIMOSXCollisionActionType)actionType;
+(VSCIMOSXCollisionActionType) actionTypeForMenuItemString:(NSString*)menuItemString;

-(void) setupInterface;

@end


@implementation VSCIMOSXCollisionActionView

@synthesize collisionAction = _collisionAction;

+(void) initialize
{
    if (actionTypeMenuItemStringDict) {
        actionTypeMenuItemStringDict = @{
        @((int)VSCIMOSXCollisionActionTypeMIDINoteOn)           : @"MIDI Note On",
        @((int)VSCIMOSXCollisionActionTypeMIDINoteOnAndOff)     : @"MIDI Note On and Off",
        @((int)VSCIMOSXCollisionActionTypeMIDINoteOff)          : @"MIDI Note Off",
        @((int)VSCIMOSXCollisionActionTypeMIDIControlChange)    : @"MIDI Control Change"
        };
    }
}

+(NSString*) stringForActionTypeMenuItem:(VSCIMOSXCollisionActionType)actionType
{
    
}

+(VSCIMOSXCollisionActionType) actionTypeForMenuItemString:(NSString*)menuItemString
{
    
}

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

#pragma mark - UI Callbacks

-(IBAction) actionTypeChanged:(id)sender
{
    
}

#pragma mark - Custom Setter

-(void) setCollisionAction:(VSC::IM::CollisionAction::WPtr)action {
    
    _collisionAction = action;
    
    self.currentActionType = VSCIMOSXCollisionActionTypeForCollisionAction(_collisionAction.lock());
    
    [self setupInterface];
    
}

#pragma mark - UI Helper

-(void) setupInterface
{
    switch (self.currentActionType) {
            
        case VSCIMOSXCollisionActionTypeMIDINoteOn:
            break;
            
        case VSCIMOSXCollisionActionTypeMIDINoteOnAndOff:
            break;
            
        case VSCIMOSXCollisionActionTypeMIDINoteOff:
            break;
            
        case VSCIMOSXCollisionActionTypeMIDIControlChange:
            break;
            
        default:
            break;
    }
}



@end
