//
//  VSCIMOSXCollisionEventChainView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/29/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionEventChainView.h"
#import "PXListViewCell.h"

#include <boost/assert.hpp>

@interface VSCIMOSXCollisionEventChainView ()

-(void) prependEvent:(VSC::IM::Event::SPtr)event;
-(void) appendEvent:(VSC::IM::Event::SPtr)event;
-(void) insertEventBeforeSelection:(VSC::IM::Event::SPtr)event;
-(void) insertEventAfterSelection:(VSC::IM::Event::SPtr)event;

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
    BOOST_ASSERT(self.collisionEventListView);
    BOOST_ASSERT(self.addEventButton);
    BOOST_ASSERT(self.removeEventButton);
    BOOST_ASSERT(self.addEventMenu);
    
    self.collisionEventListView.allowsEmptySelection = YES;
    self.collisionEventListView.allowsMultipleSelection = NO;
}

#pragma mark - Event Select/Add/Remove

-(VSC::IM::Event::SPtr) selectedEvent
{
    NSIndexSet* indexSet = [self.collisionEventListView selectedRows];
    
    BOOST_ASSERT([indexSet count] <= 1);
    
    if ([indexSet count] == 1)
    {
        NSUInteger index = [indexSet firstIndex];
        VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
        if (chain) return chain->getEvents().at(index);
    }
    
    return VSC::IM::Event::SPtr();
}

-(IBAction) addEvent:(id)sender
{

}

-(IBAction) removeSelectedEvent:(id)sender
{
    VSC::IM::Event::SPtr selectedEvent = [self selectedEvent];
    
    if (selectedEvent)
    {
        VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
        if (chain)
        {
            chain->removeEvent(selectedEvent);
            [self.collisionEventListView reloadData];
        }
    }
}

#pragma mark - Collision Actions 

-(IBAction) prependNewCollisionAction:(id)sender
{
    VSC::IM::Event::SPtr voidAction = VSC::IM::Event::SPtr(new VSC::IM::CollisionVoidAction);
    [self prependEvent:voidAction];
}

-(IBAction) appendNewCollisionAction:(id)sender
{
    VSC::IM::Event::SPtr voidAction = VSC::IM::Event::SPtr(new VSC::IM::CollisionVoidAction);
    [self appendEvent:voidAction];
}

-(IBAction) insertNewCollisionActionBeforeSelection:(id)sender
{
    VSC::IM::Event::SPtr voidAction = VSC::IM::Event::SPtr(new VSC::IM::CollisionVoidAction);
    [self insertEventBeforeSelection:voidAction];
}

-(IBAction) insertNewCollisionActionAfterSelection:(id)sender
{
    VSC::IM::Event::SPtr voidAction = VSC::IM::Event::SPtr(new VSC::IM::CollisionVoidAction);
    [self insertEventAfterSelection:voidAction];
}

#pragma mark - Delays

-(IBAction) prependNewDelay:(id)sender
{
    VSC::IM::Event::SPtr delay = VSC::IM::Event::SPtr(new VSC::IM::Delay);
    [self prependEvent:voidAction];
}

-(IBAction) appendNewDelay:(id)sender
{
    VSC::IM::Event::SPtr delay = VSC::IM::Event::SPtr(new VSC::IM::Delay);
    [self appendEvent:delay];
}

-(IBAction) insertNewDelayBeforeSelection:(id)sender
{
    VSC::IM::Event::SPtr delay = VSC::IM::Event::SPtr(new VSC::IM::Delay);
    [self insertEventBeforeSelection:delay];
}

-(IBAction) insertNewDelayAfterSelection:(id)sender
{
    VSC::IM::Event::SPtr delay = VSC::IM::Event::SPtr(new VSC::IM::Delay);
    [self insertEventAfterSelection:delay];
}

#pragma mark - Helpers

-(void) prependEvent:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    if (chain && event)
    {
        chain->prependEvent(event);
        [self.collisionEventListView reloadData];
    }
}

-(void) appendEvent:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    if (chain && event)
    {
        chain->appendEvent(event);
        [self.collisionEventListView reloadData];
    }
}

-(void) insertEventBeforeSelection:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    VSC::IM::Event::SPtr selectedEvent = [self selectedEvent];
    if (chain && selectedEvent && event)
    {
        chain->insertEventBeforeEvent(event, selectedEvent);
        [self.collisionEventListView reloadData];
    }
}

-(void) insertEventAfterSelection:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    VSC::IM::Event::SPtr selectedEvent = [self selectedEvent];
    if (chain && selectedEvent && event)
    {
        chain->insertEventAfterEvent(event, selectedEvent);
        [self.collisionEventListView reloadData];
    }
}

@end
