//
//  VSCIMOSXCollisionEventChainViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionEventChainViewController.h"

#import "VSCOSXOBSceneElementEditor.h"
#import "VSCOSXOBSceneElementController.h"
#import "VSCIMOSXCollisionActionMappingsViewController.h"
#import "VSCIMOSXCollisionActionView.h"
#import "VSCIMOSXDelayView.h"

#import "PXListView.h"
#import "PXListViewCell.h"

#include "VSCIMCollisionMapper.h"
#include "VSCIMEvent.h"
#include "VSCIMDelay.h"
#include "VSCIMAction.h"
#include "VSCIMCollisionMIDIActions.h"
#include "VSCIMCollisionEventChain.h"

#include <boost/assert.hpp>

NSString* const VSCIMOSXCollisionActionViewReuseIdentifier      = @"VSCIMOSXCollisionActionViewReuseIdentifier";
NSString* const VSCIMOSXDelayViewReuseIdentifier                = @"VSCIMOSXDelayViewReuseIdentifier";

@interface VSCIMOSXCollisionEventChainViewController ()

+(VSCIMOSXCollisionMappingView*) newCollisionMappingView;
+(VSCIMOSXCollisionActionView*) newCollisionActionView;
+(VSCIMOSXDelayView*) newDelayView;

-(void) customInit;

-(void) appendEvent:(VSC::IM::Event::SPtr)event;
-(void) removeEvent:(VSC::IM::Event::SPtr)event;

@end

@implementation VSCIMOSXCollisionEventChainViewController

const static BOOL traceInterface = YES;

@synthesize elementController = _elementController;
@synthesize eventChain = _eventChain;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        // Initialization code here.
        [self customInit];
    }
    
    return self;
}


-(void) customInit
{
    
}

- (void) awakeFromNib
{
    BOOST_ASSERT(self.view);
    self.view.translatesAutoresizingMaskIntoConstraints = NO;
    
    BOOST_ASSERT(self.eventListView);
    self.eventListView.translatesAutoresizingMaskIntoConstraints = NO;
    
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
        VSC::IM::CollisionEventChain::SPtr chain = self.eventChain.lock();
        if (chain) return chain->getEvents().at(index);
    }
    
    return VSC::IM::Event::SPtr();
}

-(IBAction) removeSelectedEvent:(id)sender
{
    VSC::IM::Event::SPtr selectedEvent = [self selectedChainEvent];
    
    if (selectedEvent)
    {
        [self removeEvent:selectedEvent];
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
        [self appendEvent:event];
    }
}

-(void) appendEvent:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.eventChain.lock();
    
    BOOST_ASSERT(chain);
    BOOST_ASSERT(event);
    
    if (chain && event)
    {
        chain->appendEvent(event);
        [self reloadInterface];
    }
}

-(void) removeEvent:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = self.eventChain.lock();
    
    BOOST_ASSERT(chain);
    BOOST_ASSERT(event);
    
    if (chain && event)
    {
        chain->removeEvent(event);
        [self reloadInterface];
    }
    
}

#pragma mark - UI Helpers

-(void) sender:(id)sender requestsMappingEditorForAction:(VSC::IM::CollisionAction::SPtr)action
{
    if (!self.actionMappingsViewController)
    {
        self.actionMappingsViewController = [[VSCIMOSXCollisionActionMappingsViewController alloc]
                                             initWithNibName:@"VSCIMOSXCollisionActionMappingsViewController" bundle:nil];
        
        BOOST_ASSERT(self.actionMappingsViewController);
        
        NSView* actionMappingsView = self.actionMappingsViewController.view;
                                             
        NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(actionMappingsView);
        [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[actionMappingsView]|"
                                                                          options:0
                                                                          metrics:nil
                                                                            views:viewsDictionary]];
        [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[actionMappingsView]|"
                                                                          options:0
                                                                          metrics:nil
                                                                            views:viewsDictionary]];
    }
    
    self.actionMappingsViewController.action = VSC::IM::CollisionAction::WPtr(action);
    
    if ([self.actionMappingsViewController.view superview] != self.view)
    {
        [self.view addSubview:self.actionMappingsViewController.view];
    }
    
}

-(void) senderRequestsEventChainView:(id)sender
{
    if ([self.actionMappingsViewController.view superview] == self.view)
    {
        [self.actionMappingsViewController.view removeFromSuperview];
    }
}


#pragma mark - View Factory Methods



+(VSCIMOSXCollisionActionView*) newCollisionActionView
{
    VSCIMOSXCollisionActionView* cell = [VSCIMOSXCollisionActionView cellLoadedFromNibNamed:@"VSCIMOSXCollisionActionView"
                                                                                     bundle:nil
                                                                         reusableIdentifier:VSCIMOSXCollisionActionViewReuseIdentifier];
    
    BOOST_ASSERT(cell);
    
    return cell;
}

+(VSCIMOSXDelayView*) newDelayView
{
    VSCIMOSXDelayView* cell = [VSCIMOSXDelayView cellLoadedFromNibNamed:@"VSCIMOSXDelayView"
                                                                 bundle:nil
                                                     reusableIdentifier:VSCIMOSXDelayViewReuseIdentifier];
    
    BOOST_ASSERT(cell);
    
    return cell;
}




#pragma mark - PXListViewDelegate

- (NSUInteger)numberOfRowsInListView:(PXListView*)aListView
{
    BOOST_ASSERT(aListView == self.eventListView);
    
    if (aListView == self.eventListView)
    {
        VSC::IM::CollisionEventChain::SPtr chain = self.eventChain.lock();
        //BOOST_ASSERT(chain);
        if (chain) return chain->getNumberOfEvents();
        else if (traceInterface) NSLog(@"%@ numberOfRowsInListView for %@ NO CHAIN", self, aListView);
    }
    
	return 0;
}

- (PXListViewCell*)listView:(PXListView*)aListView cellForRow:(NSUInteger)row
{
    BOOST_ASSERT(aListView == self.eventListView);
    
    if (aListView == self.eventListView)
    {
        VSC::IM::CollisionEventChain::SPtr chain = self.eventChain.lock();
        BOOST_ASSERT(chain);
        VSC::IM::Event::SPtr event = VSC::IM::Event::SPtr();
        if (chain) event = chain->getEventAtIndex((unsigned int)row);
        
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
                actionView = [[self class] newCollisionActionView];
            }
            [actionView setCollisionAction:(VSC::IM::CollisionAction::WPtr(action))];
            actionView.eventChainController = self;
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
                delayView = [[self class] newDelayView];
            }
            [delayView setDelay:(VSC::IM::Delay::WPtr(delay))];
            return delayView;
        }
        
    }
    	
	return nil;
}

- (CGFloat)listView:(PXListView*)aListView heightOfRow:(NSUInteger)row
{
    
    BOOST_ASSERT(aListView == self.eventListView);
    
    if (aListView == self.eventListView)
    {
        VSC::IM::CollisionEventChain::SPtr chain = self.eventChain.lock();
        BOOST_ASSERT(chain);
        VSC::IM::Event::SPtr event = VSC::IM::Event::SPtr();
        if (chain) event = chain->getEventAtIndex((unsigned int)row);
        
        VSC::IM::CollisionAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionAction>(event);
        if (action) return [VSCIMOSXCollisionActionView heightOfViewForCollisionAction:action];
        
        VSC::IM::Delay::SPtr delay = boost::dynamic_pointer_cast<VSC::IM::Delay>(event);
        if (delay) return [VSCIMOSXDelayView defaultViewHeight];
        
        BOOST_ASSERT_MSG(false, "Expected CollisionAction or Delay");
    }
    
    return 0;
}

- (void)listViewSelectionDidChange:(NSNotification*)aNotification
{
    NSLog(@"Selection changed");
}




@end
