//
//  VSCIMOSXCollisionEventChainController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/7/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionEventChainController.h"

#import "VSCIMOSXCollisionActionGlobal.h"
#import "VSCIMOSXCollisionEventChainView.h"
#import "VSCIMOSXCollisionActionView.h"
#import "VSCIMOSXCollisionMappingGlobal.h"
#import "VSCIMOSXCollisionMappingListView.h"
#import "VSCIMOSXCollisionMappingView.h"
#import "VSCIMOSXDelayView.h"
#import "PXListView.h"

#include "VSCIMCollisionAction.h"
#include "VSCIMCollisionMIDIActions.h"
#include "VSCIMDelay.h"

NSString* const VSCIMOSXCollisionMappingViewReuseIdentifier     = @"VSCIMOSXCollisionMappingViewReuseIdentifier";
NSString* const VSCIMOSXCollisionActionViewReuseIdentifier      = @"VSCIMOSXCollisionActionViewReuseIdentifier";
NSString* const VSCIMOSXDelayViewReuseIdentifier                = @"VSCIMOSXDelayViewReuseIdentifier";

@interface VSCIMOSXCollisionEventChainController ()

-(void) prependEvent:(VSC::IM::Event::SPtr)event;
-(void) appendEvent:(VSC::IM::Event::SPtr)event;
-(void) insertEventBeforeSelection:(VSC::IM::Event::SPtr)event;
-(void) insertEventAfterSelection:(VSC::IM::Event::SPtr)event;

@property (strong) IBOutletCollection(VSCIMOSXCollisionMappingView) NSArray* collisionMappingViews;

@property (weak) IBOutlet NSMenuItem* addDelayMenuItem;
@property (weak) IBOutlet NSMenuItem* addCollisionMIDINoteOnMenuItem;
@property (weak) IBOutlet NSMenuItem* addCollisionMIDINoteOffMenuItem;
@property (weak) IBOutlet NSMenuItem* addCollisionMIDINoteOnAndOffMenuItem;
@property (weak) IBOutlet NSMenuItem* addCollisionMIDIControlChangeMenuItem;

/*
 *  Add/Remove chain events
 */

-(IBAction) addEvent:(id)sender; // type will depend on the NSMenuItem which sent the message
-(IBAction) removeSelectedEvent:(id)sender;

@end


@implementation VSCIMOSXCollisionEventChainController

#pragma mark - Action / Mapping View Factory Methods

-(VSCIMOSXCollisionMappingView*) newCollisionMappingView
{
    VSCIMOSXCollisionMappingView* cell = [VSCIMOSXCollisionMappingView cellLoadedFromNibNamed:@"VSCIMOSXCollisionMappingView"
                                                                                       bundle:nil
                                                                           reusableIdentifier:VSCIMOSXCollisionMappingViewReuseIdentifier];
    
    BOOST_ASSERT(cell);
    
    return cell;
}

-(VSCIMOSXCollisionActionView*) newCollisionActionView
{
    VSCIMOSXCollisionActionView* cell = [VSCIMOSXCollisionActionView cellLoadedFromNibNamed:@"VSCIMOSXCollisionActionView"
                                                                                     bundle:nil
                                                                         reusableIdentifier:VSCIMOSXCollisionActionViewReuseIdentifier];
    
    BOOST_ASSERT(cell);
    
    return cell;
}

-(VSCIMOSXDelayView*) newDelayView
{
    VSCIMOSXDelayView* cell = [VSCIMOSXDelayView cellLoadedFromNibNamed:@"VSCIMOSXDelayView"
                                                                 bundle:nil
                                                     reusableIdentifier:VSCIMOSXDelayViewReuseIdentifier];
    
    BOOST_ASSERT(cell);
    
    return cell;
}

#pragma mark - Add and Remove Events To Chain

-(IBAction) addEvent:(id)sender
{
    VSC::IM::Event::SPtr event;
    
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
            [self.collisionEventChainView.eventListView reloadData];
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
        [self.collisionEventChainView.eventListView reloadData];
    }
}

-(void) appendEvent:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    if (chain && event)
    {
        chain->appendEvent(event);
        [self.collisionEventChainView.eventListView reloadData];
    }
}

-(void) insertEventBeforeSelection:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    VSC::IM::Event::SPtr selectedEvent = [self.collisionEventChainView selectedChainEvent];
    if (chain && selectedEvent && event)
    {
        chain->insertEventBeforeEvent(event, selectedEvent);
        [self.collisionEventChainView.eventListView reloadData];
    }
}

-(void) insertEventAfterSelection:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.collisionEventChain.lock();
    VSC::IM::Event::SPtr selectedEvent = [self.collisionEventChainView selectedChainEvent];
    if (chain && selectedEvent && event)
    {
        chain->insertEventAfterEvent(event, selectedEvent);
        [self.collisionEventChainView.eventListView reloadData];
    }
}

#pragma mark - PXListViewDelegate

- (NSUInteger)numberOfRowsInListView:(PXListView*)aListView
{
    if (aListView == [self.collisionEventChainView eventListView])
    {
        VSC::IM::CollisionEventChain::SPtr eventChain = self.collisionEventChain.lock();
        if (eventChain) return eventChain->getNumberOfEvents();
    }
    
    else if (aListView == self.collisionMappingListView.mappingListView)
    {
        if (self.collisionMappingListView && self.collisionMappingListView.collisionAction.lock())
        {
            VSC::IM::CollisionAction::SPtr collisionAction = self.collisionMappingListView.collisionAction.lock();
            return collisionAction->getExpectedMappingTargets().size();
        }
    }
    
	return 0;
}

- (PXListViewCell*)listView:(PXListView*)aListView cellForRow:(NSUInteger)row
{
    if (aListView == [self.collisionEventChainView eventListView])
    {
        VSC::IM::CollisionEventChain::SPtr eventChain = self.collisionEventChain.lock();
        VSC::IM::Event::SPtr event = eventChain->getEventAtIndex((unsigned int)row);
        if (!event) return nil;

        VSC::IM::CollisionAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionAction>(event);
        
        if (action)
        {
            PXListViewCell* cell = [aListView dequeueCellWithReusableIdentifier:VSCIMOSXCollisionActionViewReuseIdentifier];
            VSCIMOSXCollisionActionView* actionView = nil;
            
            if (cell)
            {
                BOOST_ASSERT([cell isKindOfClass:[VSCIMOSXCollisionActionView class]]);
                actionView = (VSCIMOSXCollisionActionView*)cell;
            }
            
            if(!actionView)
            {
                actionView = [self newCollisionActionView];
            }
            
            [actionView setCollisionAction:(VSC::IM::CollisionAction::WPtr(action))];
            
            return actionView;
        }
        
        VSC::IM::Delay::SPtr delay = boost::dynamic_pointer_cast<VSC::IM::Delay>(event);

        if (delay)
        {
            PXListViewCell* cell = [aListView dequeueCellWithReusableIdentifier:VSCIMOSXDelayViewReuseIdentifier];
            VSCIMOSXDelayView* delayView = nil;
            
            if (cell)
            {
                BOOST_ASSERT([cell isKindOfClass:[VSCIMOSXDelayView class]]);
                delayView = (VSCIMOSXDelayView*)cell;
            }
            
            if(!delayView)
            {
                delayView = [self newDelayView];
            }
            
            [delayView setDelay:(VSC::IM::Delay::WPtr(delay))];
            
            return delayView;
        }

    }
    
    else if (aListView == self.collisionMappingListView.mappingListView)
    {
        VSC::IM::CollisionAction::SPtr collisionAction = self.collisionMappingListView.collisionAction.lock();
        
        if (collisionAction)
        {
            const VSC::IM::Targets& targets = collisionAction->getExpectedMappingTargets();
            BOOST_ASSERT(targets.size() > row);
            if (targets.size() <= row) return nil;
            VSC::IM::Target target = targets.at(row);
            VSC::IM::CollisionMapping::SPtr collisionMapping = collisionAction->getMappingForTarget(target);
            
            if (collisionMapping)
            {
                PXListViewCell* cell = [aListView dequeueCellWithReusableIdentifier:VSCIMOSXCollisionMappingViewReuseIdentifier];
                VSCIMOSXCollisionMappingView* mappingView = nil;
                
                if (cell)
                {
                    BOOST_ASSERT([cell isKindOfClass:[VSCIMOSXCollisionMappingView class]]);
                    mappingView = (VSCIMOSXCollisionMappingView*)cell;
                }
                
                if(!mappingView)
                {
                    mappingView = [self newCollisionMappingView];
                }
                
                [mappingView setCollisionMapping:(VSC::IM::CollisionMapping::WPtr(collisionMapping))];
                
                return mappingView;
            }
        }
    }
	
	return nil;
}

- (CGFloat)listView:(PXListView*)aListView heightOfRow:(NSUInteger)row
{
    if (aListView == [self.collisionEventChainView eventListView])
    {
        VSC::IM::CollisionEventChain::SPtr eventChain = self.collisionEventChain.lock();
        VSC::IM::Event::SPtr event = eventChain->getEventAtIndex((unsigned int)row);
        if (!event) return nil;
        
        VSC::IM::CollisionAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionAction>(event);
        
        if (action)
        {
            return [VSCIMOSXCollisionActionView heightOfViewForCollisionAction:action];
        }
        
        VSC::IM::Delay::SPtr delay = boost::dynamic_pointer_cast<VSC::IM::Delay>(event);
        
        if (delay)
        {
            return [VSCIMOSXDelayView defaultViewHeight];
        }
        
    }
    
    else if (aListView == self.collisionMappingListView.mappingListView)
    {
        VSC::IM::CollisionAction::SPtr collisionAction = self.collisionMappingListView.collisionAction.lock();
        
        if (collisionAction)
        {
            const VSC::IM::Targets& targets = collisionAction->getExpectedMappingTargets();
            BOOST_ASSERT(targets.size() > row);
            if (targets.size() <= row) return 0;
            VSC::IM::Target target = targets.at(row);
            VSC::IM::CollisionMapping::SPtr collisionMapping = collisionAction->getMappingForTarget(target);
            
            return [VSCIMOSXCollisionMappingView heightOfViewForCollisionMapping:collisionMapping];
        }
    }
}

- (void)listViewSelectionDidChange:(NSNotification*)aNotification
{
    NSLog(@"Selection changed");
}



@end
