//
//  VSCIMOSXCollisionEventChainView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/29/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionEventChainView.h"
#import "VSCIMOSXCollisionEventChainController.h"
#import "VSCOSXOBSceneElementEditor.h"
#import "VSCOSXOBSceneElementController.h"

#import "PXListView.h"
#import "PXListViewCell.h"

#include "VSCIMEvent.h"
#include "VSCIMDelay.h"
#include "VSCIMAction.h"
#include "VSCIMCollisionMIDIActions.h"
#include "VSCIMCollisionEventChain.h"

#include <boost/assert.hpp>

@interface VSCIMOSXCollisionEventChainView ()


@end

@implementation VSCIMOSXCollisionEventChainView

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

- (void) awakeFromNib
{
    BOOST_ASSERT(self.eventListView);
    BOOST_ASSERT(self.addEventButton);
    BOOST_ASSERT(self.removeEventButton);
    BOOST_ASSERT(self.addEventMenu);
    
    self.eventListView.allowsEmptySelection = YES;
    self.eventListView.allowsMultipleSelection = NO;
}

-(void) reloadInterface
{
    [self.eventListView reloadData];
}

#pragma mark - Event Select/Add/Remove

-(VSC::IM::Event::SPtr) selectedChainEvent
{
    NSIndexSet* indexSet = [self.eventListView selectedRows];
    
    BOOST_ASSERT([indexSet count] <= 1);
    
    if ([indexSet count] == 1)
    {
        NSUInteger index = [indexSet firstIndex];
        
        if (self.eventChainController)
        {
            VSC::IM::CollisionEventChain::SPtr chain = [self.eventChainController collisionEventChain].lock();
            if (chain) return chain->getEvents().at(index);
        }
    }
    
    return VSC::IM::Event::SPtr();
}

-(IBAction) removeSelectedEvent:(id)sender
{
    VSC::IM::Event::SPtr selectedEvent = [self.collisionEventChainView selectedChainEvent];
    
    if (selectedEvent)
    {
        BOOST_ASSERT(self.elementController);
        BOOST_ASSERT([self.elementController respondsToSelector:@selector(collisionEventChainEditor:requestsRemovingEvent:)]);
        
        if ([self.elementController respondsToSelector:@selector(collisionEventChainEditor:requestsRemovingEvent:)])
        {
            [self.elementController collisionEventChainEditor:self requestsRemovingEvent:event];
        }
    }
}

-(IBAction) menuItemAction:(id)sender
{
    VSC::IM::Event::SPtr event = VSC::IM::Event::SPtr();
    
    if (sender == self.addDelayMenuItem)
        event = VSC::IM::Event::SPtr(new VSC::IM::Delay);
    else if (sender == self.addCollisionMIDINoteOnMenuItem)
        event = VSC::IM::Event::SPtr(new VSC::IM::CollisionMIDINoteOnAction);
    else if (sender == self.addCollisionMIDINoteOffMenuItem)
        event = VSC::IM::Event::SPtr(new VSC::IM::CollisionMIDINoteOffAction);
    else if (sender == self.addCollisionMIDINoteOnAndOffMenuItem)
        event = VSC::IM::Event::SPtr(new VSC::IM::CollisionMIDINoteOnAndOffAction);
    else if (sender == self.addCollisionMIDIControlChangeMenuItem)
        event = VSC::IM::Event::SPtr(new VSC::IM::CollisionMIDIControlChangeAction);
    
    if (event)
    {
        BOOST_ASSERT(self.elementController);
        BOOST_ASSERT([self.elementController respondsToSelector:@selector(collisionEventChainEditor:requestsAppendingEvent:)]);
        
        if ([self.elementController respondsToSelector:@selector(collisionEventChainEditor:requestsAppendingEvent:)])
        {
            [self.elementController collisionEventChainEditor:self requestsAppendingEvent:event];
        }
    }
}


@end
