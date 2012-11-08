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

@property (nonatomic, strong) IBOutlet NSMenuItem* addDelayMenuItem;

@property (nonatomic, strong) IBOutlet NSMenuItem* addCollisionMIDINoteOnMenuItem;
@property (nonatomic, strong) IBOutlet NSMenuItem* addCollisionMIDINoteOffMenuItem;
@property (nonatomic, strong) IBOutlet NSMenuItem* addCollisionMIDINoteOnAndOffMenuItem;
@property (nonatomic, strong) IBOutlet NSMenuItem* addCollisionMIDIControlChangeMenuItem;

/*
 *  Add/Remove chain events
 */

-(IBAction) addEvent:(id)sender; // type will depend on the NSMenuItem which sent the message
-(IBAction) removeSelectedEvent:(id)sender;

@end


@implementation VSCIMOSXCollisionEventChainController


#pragma mark - Add and Remove Events To Chain

-(IBAction) addEvent:(id)sender
{
    VSC::IM::Event::SPtr event;
    
    if (sender == self.addDelayMenuItem)
        event = VSC::IM::Event::SPtr(new VSC::IM::Delay);
    else if (sender == self.addCollisionMIDINoteOnMenuItem)
        event = VSC::IM::Event::SPtr(new VSC::IM::CollisionNoteOnAction);
    else if (sender == self.addCollisionMIDINoteOffMenuItem)
        event = VSC::IM::Event::SPtr(new VSC::IM::CollisionNoteOffAction);
    else if (sender == self.addCollisionMIDINoteOnAndOffMenuItem)
        event = VSC::IM::Event::SPtr(new VSC::IM::CollisionNoteOnAndOffAction);
    else if (sender == self.addCollisionMIDIControlChangeMenuItem)
        event = VSC::IM::Event::SPtr(new VSC::IM::CollisionControlChangeAction);
    
    if (event)
    {
        [self appendEvent:event];
    }
}

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
    VSC::IM::Event::SPtr selectedEvent = [self.collisionEventChainView selectedChainEvent];
    if (chain && selectedEvent && event)
    {
        chain->insertEventBeforeEvent(event, selectedEvent);
        [self.collisionEventChainView.collisionEventListView reloadData];
    }
}

-(void) insertEventAfterSelection:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    VSC::IM::Event::SPtr selectedEvent = [self.collisionEventChainView selectedChainEvent];
    if (chain && selectedEvent && event)
    {
        chain->insertEventAfterEvent(event, selectedEvent);
        [self.collisionEventChainView.collisionEventListView reloadData];
    }
}

#pragma mark - PXListViewDelegate



@end
