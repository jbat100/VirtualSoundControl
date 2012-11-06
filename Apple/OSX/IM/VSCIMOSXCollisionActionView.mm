//
//  VSCIMCollisionActionView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionActionView.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

NSDictionary* actionTypeMenuItemStringDict = nil;

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

/*
 *  Only for MIDI actions
 */

@property (nonatomic, strong) NSPopUpButton* midiControlNumberPopUpButton;

+(NSString*) stringForActionTypeMenuItem:(VSCIMOSXCollisionActionType)actionType;
+(VSCIMOSXCollisionActionType) actionTypeForMenuItemString:(NSString*)menuItemString;

-(void) setupInterface;

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
    
    [self setupInterface];
    
}

#pragma mark - UI Helper

-(void) setupInterface
{
    
    VSC::IM::CollisionAction::SPtr action = self.collisionAction.lock();
    
    [self.actionTypePopUpButton selectItemWithTitle:[[self class] stringForActionTypeMenuItem:self.currentActionType]];
    
    if (!action)
    {
        // empty interface and return 
    }
    
    /*
     *  If action then tighten your seat belts, here we go...
     */
    
    
    
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
            //NSRect f = NSMakeRect(<#CGFloat x#>, <#CGFloat y#>, <#CGFloat w#>, <#CGFloat h#>)
            VSCIMOSXCollisionMappingView* mappingView = [[VSCIMOSXCollisionMappingView alloc] initWithFrame:];
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
            break;
            
        default:
            break;
    }
}

-(void) updateMIDIControlNumbers
{
    
}



@end
