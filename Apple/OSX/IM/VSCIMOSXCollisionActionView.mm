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

const CGFloat VSCIMOSXCollisionActionViewHorizontalMargin   = 5.0;
const CGFloat VSCIMOSXCollisionActionViewVerticalMargin     = 5.0;

const CGFloat VSCIMOSXCollisionActionViewPopUpButtonHeight  = 10.0;

const CGFloat VSCIMOSXCollisionActionViewBaseHeight         = 20.0;
const CGFloat VSCIMOSXCollisionActionViewMappingHeight      = 20.0;
const CGFloat VSCIMOSXCollisionActionViewParameterHeight    = 20.0;

/*
 *  Private internals
 */

@interface VSCIMOSXCollisionActionView ()

/*
 *  Used by all action types
 */

@property (nonatomic, assign) VSCIMOSXCollisionActionType currentActionType;
@property (nonatomic, strong) NSPopUpButton* actionTypePopUpButton;

/*
 *  Generic mapping views (not for mapping editting, just minimal)
 */

@property (nonatomic, strong) NSMutableArray* collisionMappingViews;
@property (nonatomic, strong) NSNib* mappingViewNib;

/*
 *  Only for MIDI actions
 */

@property (nonatomic, strong) NSPopUpButton* midiControlNumberPopUpButton;

-(VSCIMOSXCollisionMappingView*) newCollisionMappingView;

-(void) setupActionChoice;
-(void) setupInterfaceForNewCollisionAction;
-(void) updateMIDIControlNumbers;

@end


@implementation VSCIMOSXCollisionActionView

@synthesize collisionAction = _collisionAction;

+(CGFloat) heightOfViewForCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction
{
    CGFloat totalHeight = VSCIMOSXCollisionActionViewBaseHeight;
    
    const VSC::IM::Targets& expectedTargets = collisionAction->getExpectedMappingTargets();
    totalHeight += expectedTargets.size();
    
    VSCIMOSXCollisionActionType actionType = VSCIMOSXCollisionActionTypeForCollisionAction(collisionAction);
    
    if (VSCIMOSXCollisionActionTypeIsMIDI(actionType))
    {
        totalHeight += VSCIMOSXCollisionActionViewParameterHeight;
    }
    
    if (actionType == VSCIMOSXCollisionActionTypeMIDIControlChange)
    {
        totalHeight += VSCIMOSXCollisionActionViewParameterHeight;
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

+(NSString*) stringForActionTypeMenuItem:(VSCIMOSXCollisionActionType)actionType
{
    BOOST_ASSERT(actionTypeMenuItemStringDict);
    
    return [actionTypeMenuItemStringDict objectForKey:@((int)actionType)];
}

+(VSCIMOSXCollisionActionType) actionTypeForMenuItemString:(NSString*)menuItemString
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
        
        NSRect f = NSMakeRect(VSCIMOSXCollisionActionViewHorizontalMargin,
                              frame.size.height - VSCIMOSXCollisionActionViewVerticalMargin,
                              frame.size.width - (2.0*VSCIMOSXCollisionActionViewHorizontalMargin),
                              VSCIMOSXCollisionActionViewPopUpButtonHeight);
        
        self.actionTypePopUpButton = [[NSPopUpButton alloc] initWithFrame:f];
        [self.actionTypePopUpButton setAutoresizingMask:(NSUInteger)(NSViewWidthSizable | NSViewMinYMargin)];
        [self.actionTypePopUpButton setTarget:self];
        [self.actionTypePopUpButton setAction:@selector(actionTypeChanged:)];
        
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

#pragma mark - UI Callbacks

-(void) actionTypeChanged:(id)sender
{
    VSCIMOSXCollisionActionType newActionType = [[self class] actionTypeForMenuItemString:[[self.actionTypePopUpButton selectedItem] title]];
    
    if (newActionType != self.currentActionType)
    {
        VSC::IM::CollisionAction::SPtr newAction = [self.controller collisionActionEditor:self requestsCollisionActionWithType:newActionType];
        self.collisionAction = VSC::IM::CollisionAction::WPtr(newAction);
    }
}

-(void) midiControlNumberChanged:(id)sender
{


}

#pragma mark - Custom Setter

-(void) setCollisionAction:(VSC::IM::CollisionAction::WPtr)action {
    
    _collisionAction = action;
    
    self.currentActionType = VSCIMOSXCollisionActionTypeForCollisionAction(_collisionAction.lock());
    
    [self setupInterfaceForNewCollisionAction];
    
}

#pragma mark - UI Helper

-(void) setupActionChoice
{
    BOOST_ASSERT(actionTypeMenuItemStringDict);
    BOOST_ASSERT(self.actionTypePopUpButton);
    
    [self.actionTypePopUpButton removeAllItems];
    
    NSArray* keys = [[actionTypeMenuItemStringDict allKeys] sortedArrayUsingSelector:@selector(compare:)];
    
    for (NSNumber* key in keys)
    {
        NSString* actionDescription = [actionTypeMenuItemStringDict objectForKey:key];
        BOOST_ASSERT([actionDescription isKindOfClass:[NSString class]]);
        [self.actionTypePopUpButton addItemWithTitle:actionDescription];
    }
}

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
