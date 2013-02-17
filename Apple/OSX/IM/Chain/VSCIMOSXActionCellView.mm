//
//  VSCIMActionView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXActionCellView.h"
#import "VSCIMOSXEventChainController.h"
#import "VSCOBOSXElementEditor.h"
#import "VSCOSXInterfaceFactory.h"
#import "NSString+VSCAdditions.h"

#include "VSCMIDI.h"
#include "VSCMIDIOutputManager.h"
#include "VSCMIDIOwners.h"

#include "VSCIMEvent.h"
#include "VSCIMAction.h"
#include "VSCIMActionImplementations.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace VSC;
using namespace VSC::IM;

/*
 *  Private internals
 */

@interface VSCIMOSXActionCellView ()
{
    
}

/*
 *  Constraints!
 */
@property (strong) IBOutlet NSLayoutConstraint* mainViewBottomConstraint;

-(void) setupInterface;

-(void) commonInit;

@end


@implementation VSCIMOSXActionCellView

static const BOOL debugDraw = NO;

#pragma mark - Static Methods

+(void) initialize
{

}

#pragma mark - NSView Methods

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self commonInit];
    }
    
    return self;
}

-(id) initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        [self commonInit];
    }
    return self;
}

-(void) commonInit
{
    self.translatesAutoresizingMaskIntoConstraints = NO;
}

- (void)drawRect:(NSRect)dirtyRect
{
    CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    if (debugDraw)
    {
        CGContextSetRGBFillColor (myContext, 1.0, 0.0, 0.0, 1.0);
        CGContextFillRect(myContext, self.bounds);
        CGContextSetGrayStrokeColor (myContext, 1.0, 1.0);
        CGContextStrokeRectWithWidth(myContext, NSRectToCGRect(self.bounds), 2.0);
    }
    else
    {
        CGContextSetRGBFillColor (myContext, 0.2, 0.2, 0.2, 1.0);
        CGContextFillRect(myContext, self.bounds);
        CGContextSetGrayStrokeColor (myContext, 0.8, 1.0);
        CGContextStrokeRectWithWidth(myContext, NSRectToCGRect(self.bounds), 2.0);
    }
}

-(void) awakeFromNib
{
    [super awakeFromNib];
    
    BOOST_ASSERT(self.actionTypePopUpButton);
    
    self.translatesAutoresizingMaskIntoConstraints = NO;
    self.actionTypePopUpButton.translatesAutoresizingMaskIntoConstraints = NO;
    
    /*
     *  Populate PopUpButton
     */
    
    [self.actionTypePopUpButton removeAllItems];
    
    BOOST_FOREACH(const ActionType& actionType, AllowedActionTypes())
    {
        std::string actionTypeString = StringForActionType(actionType);
        [self.actionTypePopUpButton addItemWithTitle:[NSString stringWithStdString:actionTypeString]];
    }
    
    /*
     *  If we already have an event (can we?) then update the type. Don't use the action accessor because
     *  it asserts the action (and it will most likely be null).
     */
    
    Event::SPtr actionEvent = self.event.lock();
    Action::SPtr action = boost::dynamic_pointer_cast<Action>(actionEvent);
    
    if (action)
    {
        std::string actionTypeString = StringForActionType(action->getActionType());
        [self.actionTypePopUpButton selectItemWithTitle:[NSString stringWithStdString:actionTypeString]];
    }
    else
    {
        std::string noActionTypeString = StringForActionType(ActionTypeVoid);
        [self.actionTypePopUpButton selectItemWithTitle:[NSString stringWithStdString:noActionTypeString]];
    }
}

#pragma mark - Action Getter

-(Action::SPtr) action
{
    Event::SPtr actionEvent = self.event.lock();
    BOOST_ASSERT(actionEvent);
    Action::SPtr action = boost::dynamic_pointer_cast<Action>(actionEvent);
    BOOST_ASSERT(action);
    return action;
}

#pragma mark - Custom Setter

-(void) setEvent:(Event::WPtr)weakEvent
{
    [super setEvent:weakEvent];
    Action::SPtr action = boost::dynamic_pointer_cast<Action>(weakEvent.lock());
    BOOST_ASSERT(action);
    [self setupInterface];
}

-(BOOL) checkEvent:(Event::SPtr)testEvent
{
    BOOST_ASSERT(testEvent);
    if (!testEvent) return YES;
    Action::SPtr action = boost::dynamic_pointer_cast<Action>(testEvent);
    BOOST_ASSERT(action);
    if (action) return YES;
    return NO;
}

#pragma mark - UI Helper

-(void) setupInterface
{
    Action::SPtr action = [self action];
    BOOST_ASSERT(action);
    if (action)
    {
        std::string actionTypeString = StringForActionType(action->getActionType());
        [self.actionTypePopUpButton selectItemWithTitle:[NSString stringWithStdString:actionTypeString]];
    }
    else
    {
        std::string actionTypeString = StringForActionType(ActionTypeVoid);
        [self.actionTypePopUpButton selectItemWithTitle:[NSString stringWithStdString:actionTypeString]];
    }
}


@end
