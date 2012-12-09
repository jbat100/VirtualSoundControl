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

+(VSCIMOSXCollisionActionView*) newCollisionActionViewWithOwner:(id)owner;
+(VSCIMOSXDelayView*) newDelayViewWithOwner:(id)owner;

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
    
    BOOST_ASSERT(self.eventTableView);
    BOOST_ASSERT(self.eventTableView.delegate == self);
    BOOST_ASSERT(self.eventTableView.dataSource == self);
    self.eventTableView.translatesAutoresizingMaskIntoConstraints = NO;
    
    BOOST_ASSERT(self.addEventButton);
    BOOST_ASSERT(self.removeEventButton);
    BOOST_ASSERT(self.addEventMenu);
    
    BOOST_ASSERT(self.addDelayMenuItem);
    BOOST_ASSERT(self.addCollisionMIDINoteOnMenuItem);
    BOOST_ASSERT(self.addCollisionMIDINoteOffMenuItem);
    BOOST_ASSERT(self.addCollisionMIDINoteOnAndOffMenuItem);
    BOOST_ASSERT(self.addCollisionMIDIControlChangeMenuItem);
    
    BOOST_ASSERT(self.addCollisionMIDINoteOnMenuItem.target == self);
    
    self.eventTableView.allowsEmptySelection = YES;
    self.eventTableView.allowsMultipleSelection = NO;
}

-(void) reloadInterface
{
    [self.eventTableView reloadData];
}

#pragma mark - UI Callbacks


#pragma mark - Event Select/Add/Remove

-(VSC::IM::Event::SPtr) selectedChainEvent
{
    NSInteger index = [self.eventTableView selectedRow];
    
    if (index >= 0)
    {
        VSC::IM::CollisionEventChain::SPtr chain = self.eventChain.lock();
        BOOST_ASSERT(chain);
        BOOST_ASSERT(chain->getNumberOfEvents() > index);
        if (chain && chain->getNumberOfEvents() > index) return chain->getEvents().at(index);
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

-(void) sender:(id)sender requestsSetDelay:(VSC::IM::Delay::SPtr)delay toInterval:(NSTimeInterval)delayInterval
{
    BOOST_ASSERT_MSG(NO, "NOT IMPLEMENTED");
}

-(void) senderRequestsEventChainView:(id)sender
{
    if ([self.actionMappingsViewController.view superview] == self.view)
    {
        [self.actionMappingsViewController.view removeFromSuperview];
    }
}


#pragma mark - View Factory Methods

+(VSCIMOSXCollisionActionView*) newCollisionActionViewWithOwner:(id)owner
{
    
    static NSNib* nib = nil;
    static NSString* identifier = [[VSCIMOSXCollisionActionView class] description];
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        BOOST_ASSERT(!nib);
        nib = [[NSNib alloc] initWithNibNamed:[[VSCIMOSXCollisionActionView class] description] bundle:nil];
    });
    BOOST_ASSERT(nib);
    
    NSArray *objects = nil;
    VSCIMOSXCollisionActionView* v = nil;
    [nib instantiateNibWithOwner:owner topLevelObjects:&objects];
    for(id object in objects)
    {
        if([object isKindOfClass:[VSCIMOSXCollisionActionView class]]) {
            v = object;
            v.identifier = identifier;
            break;
        }
    }
    BOOST_ASSERT(v);
    BOOST_ASSERT(v.eventChainController == owner);
    //v.translatesAutoresizingMaskIntoConstraints = NO;
    return v;
}

+(VSCIMOSXDelayView*) newDelayViewWithOwner:(id)owner
{
    static NSNib* nib = nil;
    static NSString* identifier = [[VSCIMOSXDelayView class] description];
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        BOOST_ASSERT(!nib);
        nib = [[NSNib alloc] initWithNibNamed:[[VSCIMOSXDelayView class] description] bundle:nil];
    });
    BOOST_ASSERT(nib);
    
    NSArray *objects = nil;
    VSCIMOSXDelayView* v = nil;
    [nib instantiateNibWithOwner:owner topLevelObjects:&objects];
    for(id object in objects)
    {
        if([object isKindOfClass:[VSCIMOSXDelayView class]]) {
            v = object;
            v.identifier = identifier;
            break;
        }
    }
    BOOST_ASSERT(v);
    BOOST_ASSERT(v.eventChainController == owner);
    v.translatesAutoresizingMaskIntoConstraints = NO;
    return v;
}


#pragma mark - NSTableViewDelegate and NSTableViewDataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    BOOST_ASSERT(aTableView == self.eventTableView);
    
    if (aTableView == self.eventTableView)
    {
        VSC::IM::CollisionEventChain::SPtr chain = self.eventChain.lock();
        //BOOST_ASSERT(chain);
        if (chain) return chain->getNumberOfEvents();
        else if (traceInterface) NSLog(@"%@ numberOfRowsInTableView for %@ NO CHAIN", self, aTableView);
    }
    
	return 0;
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row
{
    BOOST_ASSERT(tableView == self.eventTableView);
    
    if (tableView == self.eventTableView)
    {
        if (traceInterface) NSLog(@"%@ listView:cellForRow: %ld", self, row);
        
        VSC::IM::CollisionEventChain::SPtr chain = self.eventChain.lock();
        BOOST_ASSERT(chain);
        VSC::IM::Event::SPtr event = VSC::IM::Event::SPtr();
        if (chain) event = chain->getEventAtIndex((unsigned int)row);
        
        VSC::IM::CollisionAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionAction>(event);
        if (action)
        {
            VSCIMOSXCollisionActionView* actionView = [tableView makeViewWithIdentifier:[[VSCIMOSXCollisionActionView class] description] owner:self];
            if (actionView) BOOST_ASSERT([actionView isKindOfClass:[VSCIMOSXCollisionActionView class]]);
            else actionView = [[self class] newCollisionActionViewWithOwner:self];
            [actionView setCollisionAction:(VSC::IM::CollisionAction::WPtr(action))];
            actionView.eventChainController = self;
            if (traceInterface) NSLog(@"Returning: %@ with frame: %@", actionView, NSStringFromRect(actionView.frame));
            return actionView;
        }
        
        VSC::IM::Delay::SPtr delay = boost::dynamic_pointer_cast<VSC::IM::Delay>(event);
        if (delay)
        {
            VSCIMOSXDelayView* delayView = [tableView makeViewWithIdentifier:[[VSCIMOSXDelayView class] description] owner:self];
            if (delayView) BOOST_ASSERT([delayView isKindOfClass:[VSCIMOSXDelayView class]]);
            else delayView = [[self class] newDelayViewWithOwner:self];
            [delayView setDelay:VSC::IM::Delay::WPtr(delay)];
            delayView.eventChainController = self;
            if (traceInterface) NSLog(@"Returning: %@ with frame: %@", delayView, NSStringFromRect(delayView.frame));
            return delayView;
        }
        
    }
    	
	return nil;
}

- (CGFloat)tableView:(NSTableView *)tableView heightOfRow:(NSInteger)row
{
    
    BOOST_ASSERT(tableView == self.eventTableView);
    
    if (tableView == self.eventTableView)
    {
        if (traceInterface) NSLog(@"%@ heightOfRow: %ld", self, row);
        
        VSC::IM::CollisionEventChain::SPtr chain = self.eventChain.lock();
        BOOST_ASSERT(chain);
        
        if (row >= chain->getNumberOfEvents())
        {
            NSLog(@"ASKING FOR ROW HEIGHT FOR OUT OF BOUNDS INDEX");
            return 0;
        }
        
        VSC::IM::Event::SPtr event = VSC::IM::Event::SPtr();
        if (chain) event = chain->getEventAtIndex((unsigned int)row);
        
        VSC::IM::CollisionAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionAction>(event);
        if (action)
        {
            CGFloat h = [VSCIMOSXCollisionActionView heightOfViewForCollisionAction:action];
            if (traceInterface) NSLog(@"Returning: %f", h);
            return h;
        }
        
        VSC::IM::Delay::SPtr delay = boost::dynamic_pointer_cast<VSC::IM::Delay>(event);
        if (delay)
        {
            CGFloat h = [VSCIMOSXDelayView defaultViewHeight];
            if (traceInterface) NSLog(@"Returning: %f", h);
            return h;
        }
        
        BOOST_ASSERT_MSG(false, "Expected CollisionAction or Delay");
    }
    
    return 0;
}

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
    NSLog(@"%@ tableViewSelectionDidChange: %@", self, aNotification);
    
    BOOST_ASSERT([aNotification object] == self.eventTableView);
    if ([aNotification object] != self.eventTableView) return;
    
    NSIndexSet* rowIndexes = [self.eventTableView selectedRowIndexes];
    
    [rowIndexes enumerateIndexesUsingBlock:^(NSUInteger idx, BOOL *stop) {
        NSView* v = [self.eventTableView viewAtColumn:0 row:idx makeIfNecessary:NO];
        NSLog(@"Row %ld: %@ with frame %@", idx, v, NSStringFromRect(v.frame));
    }];
    
}




@end
