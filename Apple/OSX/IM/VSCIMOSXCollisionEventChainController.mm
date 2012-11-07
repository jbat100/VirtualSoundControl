//
//  VSCIMOSXCollisionEventChainController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/7/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionEventChainController.h"

#import "VSCIMOSXCollisionEventChainView.h"
#import "PXListView.h"

#include "VSCIMCollisionAction.h"
#include "VSCIMDelay.h"

@interface VSCIMOSXCollisionEventChainController ()

-(void) prependEvent:(VSC::IM::Event::SPtr)event;
-(void) appendEvent:(VSC::IM::Event::SPtr)event;
-(void) insertEventBeforeSelection:(VSC::IM::Event::SPtr)event;
-(void) insertEventAfterSelection:(VSC::IM::Event::SPtr)event;

@end

@implementation VSCIMOSXCollisionEventChainController

#pragma mark - Collision Action View Callbacks

-(IBAction) showCollisionMappings:(id)sender
{
    
}

-(IBAction) refreshMIDIOutputs:(id)sender
{
    
}

-(IBAction) midiOutputSelected:(id)sender
{
    
}

-(IBAction) refreshMIDIControlNumbers:(id)sender
{
    
}

-(IBAction) midiControlNumberSelected:(id)sender
{
    
}

#pragma mark - Add and Remove Events To Chain

-(IBAction) removeSelectedEvent:(id)sender
{
    VSC::IM::Event::SPtr selectedEvent = [self.collisionEventChainView selectedChainEvent];
    
    if (selectedEvent)
    {
        VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
        if (chain)
        {
            chain->removeEvent(selectedEvent);
            [self.collisionEventChainView.collisionEventListView reloadData];
        }
    }
}

#pragma mark - Add Collision Actions To Chain

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

#pragma mark - Add Delays To Chain

-(IBAction) prependNewDelay:(id)sender
{
    VSC::IM::Event::SPtr delay = VSC::IM::Event::SPtr(new VSC::IM::Delay);
    [self prependEvent:delay];
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

#pragma mark - Add Events To Chain

-(void) prependEvent:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    if (chain && event)
    {
        chain->prependEvent(event);
        [self.collisionEventChainView.collisionEventListView reloadData];
    }
}

-(void) appendEvent:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    if (chain && event)
    {
        chain->appendEvent(event);
        [self.collisionEventChainView.collisionEventListView reloadData];
    }
}

-(void) insertEventBeforeSelection:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    VSC::IM::Event::SPtr selectedEvent = [self selectedChainEvent];
    if (chain && selectedEvent && event)
    {
        chain->insertEventBeforeEvent(event, selectedEvent);
        [self.collisionEventChainView.collisionEventListView reloadData];
    }
}

-(void) insertEventAfterSelection:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    VSC::IM::Event::SPtr selectedEvent = [self selectedChainEvent];
    if (chain && selectedEvent && event)
    {
        chain->insertEventAfterEvent(event, selectedEvent);
        [self.collisionEventChainView.collisionEventListView reloadData];
    }
}

@end
