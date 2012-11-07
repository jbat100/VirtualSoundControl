//
//  VSCIMCollisionActionView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionActionView.h"
#import "NSString+VSCAdditions.h"

#include "VSCIMCollisionMIDIActions.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

NSDictionary* actionTypeMenuItemStringDict = nil;

/*
 *  Heights should agree with nib
 */

const CGFloat VSCIMOSXCollisionActionViewBaseHeight = 54.0;
const CGFloat VSCIMOSXCollisionActionViewMIDISetupHeight = 48.0;
const CGFloat VSCIMOSXCollisionActionViewMIDIControlSetupHeight = 26.0;

/** // Using nib instead...

const CGFloat VSCIMOSXCollisionActionViewHorizontalMargin   = 5.0;
const CGFloat VSCIMOSXCollisionActionViewVerticalMargin     = 5.0;

const CGFloat VSCIMOSXCollisionActionViewPopUpButtonHeight  = 10.0;

const CGFloat VSCIMOSXCollisionActionViewBaseHeight         = 20.0;
const CGFloat VSCIMOSXCollisionActionViewMappingHeight      = 20.0;
const CGFloat VSCIMOSXCollisionActionViewParameterHeight    = 20.0;
 
 */

/*
 *  Private internals
 */

@interface VSCIMOSXCollisionActionView ()

/*
 *  Used by all action types
 */

@property (nonatomic, assign) VSCIMOSXCollisionActionType currentActionType;
//@property (nonatomic, strong) IBOutlet NSPopUpButton* actionTypePopUpButton;

@property (nonatomic, strong) IBOutlet NSView* mainView;
@property (nonatomic, strong) IBOutlet NSTextField* actionTypeTextField;
@property (nonatomic, strong) IBOutlet NSButton* mappingsButton;

-(void) setupInterfaceForNewCollisionAction;

/*
 *  Only for MIDI actions
 */

@property (nonatomic, strong) IBOutlet NSView* midiSetupView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiOutputPopUpButton;

/*
 *  Only for MIDI control actions
 */

@property (nonatomic, strong) IBOutlet NSView* midiControlSetupView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiControlNumberPopUpButton;

-(void) updateMIDIControlNumbers;

@end


@implementation VSCIMOSXCollisionActionView

@synthesize collisionAction = _collisionAction;

+(CGFloat) heightOfViewForCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction 
{
    VSCIMOSXCollisionActionType actionType = VSCIMOSXCollisionActionTypeForCollisionAction(collisionAction);
    
    CGFloat totalHeight = VSCIMOSXCollisionActionViewBaseHeight;
    
    if (VSCIMOSXCollisionActionTypeIsMIDI(actionType))
    {
        totalHeight += VSCIMOSXCollisionActionViewMIDISetupHeight;
    }
    
    if (actionType == VSCIMOSXCollisionActionTypeMIDIControlChange)
    {
        totalHeight += VSCIMOSXCollisionActionViewMIDIControlSetupHeight;
    }
    
    return totalHeight;
}

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

+(NSString*) stringForActionType:(VSCIMOSXCollisionActionType)actionType
{
    BOOST_ASSERT(actionTypeMenuItemStringDict);
    
    return [actionTypeMenuItemStringDict objectForKey:@((int)actionType)];
}

+(VSCIMOSXCollisionActionType) actionTypeForString:(NSString*)menuItemString
{
    BOOST_ASSERT(actionTypeMenuItemStringDict);
    
    NSSet* types = [actionTypeMenuItemStringDict keysOfEntriesPassingTest:^BOOL(id key, id obj, BOOL *stop) {
        if ([obj isKindOfClass:[NSString class]] == NO) return false;
        if ([(NSString*)obj isEqualToString:menuItemString]) return true;
        return false;
    }];
    
    BOOST_ASSERT([types count] < 2);
    
    if ([types count] == 0) return VSCIMOSXCollisionActionTypeNone;
    
    return (VSCIMOSXCollisionActionType)[[types anyObject] intValue];
}

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
        
        self.currentActionType = VSCIMOSXCollisionActionTypeNone;
        
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}




#pragma mark - Custom Setter

-(void) setCollisionAction:(VSC::IM::CollisionAction::WPtr)action {
    
    _collisionAction = action;
    
    self.currentActionType = VSCIMOSXCollisionActionTypeForCollisionAction(_collisionAction.lock());
    
    [self setupInterfaceForNewCollisionAction];
    
}

#pragma mark - UI Helper



-(void) setupInterfaceForNewCollisionAction
{
    
    VSC::IM::CollisionAction::SPtr action = self.collisionAction.lock();
    
    /*
     *  If action then tighten your seat belts, here we go...
     */
    
    CGFloat h = [[self class] heightOfViewForCollisionAction:action];
    
    [self.actionTypePopUpButton selectItemWithTitle:[[self class] stringForActionTypeMenuItem:self.currentActionType]];
    
    if (!action)
    {
        // empty interface and return 
    }
    
    
    /*
     *  Empty current mapping views
     */
    
    for (VSCIMOSXCollisionMappingView* mappingView in self.collisionMappingViews)
    {
        BOOST_ASSERT([mappingView isKindOfClass:[VSCIMOSXCollisionMappingView class]]);
        [mappingView removeFromSuperview];
    }
    
    [self.collisionMappingViews removeAllObjects];
    
    /*
     *  Create new mapping views
     */
    
    const VSC::IM::Targets& targets = action->getExpectedMappingTargets();
    
    BOOST_FOREACH(const VSC::IM::Target& target, targets)
    {
        VSC::IM::CollisionMapping::SPtr mapping = action->getMappingForTarget(target);
        BOOST_ASSERT(mapping);
        if (mapping)
        {
            NSRect f = NSMakeRect(5.0, h - 5.0, self.frame.size.width - 10.0, 10.0);
            VSCIMOSXCollisionMappingView* mappingView = [[VSCIMOSXCollisionMappingView alloc] initWithFrame:f];
            mappingView.target = target;
            mappingView.collisionMapping = VSC::IM::CollisionMapping::WPtr(mapping);
            [mappingView setAutoresizingMask:(NSUInteger)(NSViewWidthSizable | NSViewMinYMargin)];
        }
    }
    
    switch (self.currentActionType) {
            
        case VSCIMOSXCollisionActionTypeMIDINoteOn:
            break;
            
        case VSCIMOSXCollisionActionTypeMIDINoteOnAndOff:
            break;
            
        case VSCIMOSXCollisionActionTypeMIDINoteOff:
            break;
            
        case VSCIMOSXCollisionActionTypeMIDIControlChange:
            [self updateMIDIControlNumbers];
            break;
            
        default:
            break;
    }
}

-(void) updateMIDIControlNumbers
{
    //BOOST_ASSERT(self.currentActionType == VSCIMOSXCollisionActionTypeMIDIControlChange);
    
    [self.midiControlNumberPopUpButton removeAllItems];
    
    VSC::IM::CollisionMIDIAction::SPtr midiAction = boost::dynamic_pointer_cast<VSC::IM::CollisionMIDIAction>(self.collisionAction.lock());
    
    BOOST_ASSERT(midiAction);
    
    VSC::MIDI::Output::SPtr midiOutput = midiAction->getMIDIOuput();
    
    if (midiOutput)
    {
        const VSC::MIDI::ControlNumbers& controlNumbers = midiOutput->getValidControlNumbers();
        
        BOOST_FOREACH(const VSC::MIDI::ControlNumber& controlNumber, controlNumbers)
        {
            std::string controlNumberString = VSC::MIDI::controlNumberToString(controlNumber);
            BOOST_ASSERT(!controlNumberString.empty());
            if (!controlNumberString.empty())
            {
                [self.midiControlNumberPopUpButton addItemWithTitle:[NSString stringWithStdString:controlNumberString]];
            }
        }
    }
}



@end
