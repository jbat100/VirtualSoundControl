//
//  VSCIMOSXEventChainViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXEventChainViewController.h"

#import "VSCOBOSXElementEditor.h"
#import "VSCOBOSXElementController.h"
#import "VSCIMOSXEventEditorViewController.h"
#import "VSCIMOSXActionCellView.h"
#import "VSCIMOSXDelayCellView.h"

#include "VSCCollisionMapper.h"

#include "VSCIM.h"
#include "VSCIMEvent.h"
#include "VSCIMDelay.h"
#include "VSCIMAction.h"
#include "VSCIMActionImplementations.h"
#include "VSCIMEventChain.h"

#include <boost/assert.hpp>

using namespace VSC;
using namespace VSC::IM;

NSString* const VSCIMOSXActionCellViewReuseIdentifier      = @"VSCIMOSXActionCellViewReuseIdentifier";
NSString* const VSCIMOSXDelayCellViewReuseIdentifier       = @"VSCIMOSXDelayCellViewReuseIdentifier";

@interface VSCIMOSXEventChainViewController ()

@property (nonatomic, strong) NSArray* mainViewConstraints;

-(void) customInit;

-(void) resetView;
-(void) switchToView:(NSView*)newView;

-(void) appendEvent:(Event::SPtr)event;
-(void) removeEvent:(Event::SPtr)event;

@end

@implementation VSCIMOSXEventChainViewController

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
    self.addEventButton.translatesAutoresizingMaskIntoConstraints = NO;
    self.removeEventButton.translatesAutoresizingMaskIntoConstraints = NO;
    
    BOOST_ASSERT(self.addDelayMenuItem);
    BOOST_ASSERT(self.addActionMenuItem);
    
    self.eventTableView.allowsEmptySelection = YES;
    self.eventTableView.allowsMultipleSelection = NO;
}

-(void) reloadInterface
{
    [self.eventTableView reloadData];
}

#pragma mark - Debugging

-(void) printUIDescription
{
    //NSArray* actionViews = [self.eventTableView ]
}

#pragma mark - UI Callbacks



#pragma mark - Event Select/Add/Remove

-(Event::SPtr) selectedChainEvent
{
    NSInteger index = [self.eventTableView selectedRow];
    
    if (index >= 0)
    {
        EventChain::SPtr chain = self.eventChain.lock();
        BOOST_ASSERT(chain);
        BOOST_ASSERT(chain->getNumberOfEvents() > index);
        if (chain && chain->getNumberOfEvents() > index) return chain->getEvents().at(index);
    }
    
    return Event::SPtr();
}

-(IBAction) removeSelectedEvent:(id)sender
{
    Event::SPtr selectedEvent = [self selectedChainEvent];
    
    if (selectedEvent)
    {
        [self removeEvent:selectedEvent];
    }
}

-(IBAction) menuItemAction:(id)sender
{
    Event::SPtr event = Event::SPtr();
    
    if (sender == self.addDelayMenuItem)
    {
        event = Event::SPtr(new IM::Delay);
    }
    else if (sender == self.addActionMenuItem)
    {
        event = Event::SPtr(new Action);
        Action::SPtr action = boost::dynamic_pointer_cast<Action>(event);
        action->setActionType(ActionTypeVoid);
    }
    if (event)
    {
        [self appendEvent:event];
    }
}

-(void) appendEvent:(Event::SPtr)event
{
    EventChain::SPtr chain = self.eventChain.lock();
    
    BOOST_ASSERT(chain);
    BOOST_ASSERT(event);
    
    if (chain && event)
    {
        chain->appendEvent(event);
        [self reloadInterface];
    }
}

-(void) removeEvent:(Event::SPtr)event
{
    EventChain::SPtr chain = self.eventChain.lock();
    
    BOOST_ASSERT(chain);
    BOOST_ASSERT(event);
    
    if (chain && event)
    {
        chain->removeEvent(event);
        [self reloadInterface];
    }
    
}

#pragma mark - UI Helpers

-(void) switchToView:(NSView*)newView
{
    [self resetView];
    
    if (newView == nil) return;
    
    BOOST_ASSERT(newView);
    BOOST_ASSERT([newView isKindOfClass:[NSView class]]);
    
    if ([newView isKindOfClass:[NSView class]] == NO) return;
    if ([newView superview] == self.view) return;
    
    if (self.mainViewConstraints)
    {
        [self.view removeConstraints:self.mainViewConstraints];
    }
    
    [self.view addSubview:newView];
    
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(newView);
    
    NSMutableArray* allConstraints = [NSMutableArray array];
    
    NSArray* hConstraints =[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[newView]-0-|"
                                                                   options:0
                                                                   metrics:nil
                                                                     views:viewsDictionary];
    
    [allConstraints addObjectsFromArray:hConstraints];
    [self.view addConstraints:hConstraints];
    
    NSArray* vConstraints =[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[newView]-0-|"
                                                                   options:0
                                                                   metrics:nil
                                                                     views:viewsDictionary];
    
    [allConstraints addObjectsFromArray:vConstraints];
    [self.view addConstraints:vConstraints];
    
    self.mainViewConstraints = [NSArray arrayWithArray:allConstraints];
}



-(void) sender:(id)sender requestsShowEventEditorForEvent:(Event::SPtr)event
{
    if (!self.eventEditorViewController)
    {
        self.eventEditorViewController = [[VSCIMOSXEventEditorViewController alloc]
                                             initWithNibName:@"VSCIMOSXEventEditorViewController" bundle:nil];
        
        BOOST_ASSERT(self.eventEditorViewController);
        self.eventEditorViewController.eventChainController = self;
    }
    
    BOOST_ASSERT(event);
    self.eventEditorViewController.event = Event::WPtr(event);
    
    if ([self.eventEditorViewController.view superview] != self.view)
    {
        NSView* eventEditorView = self.eventEditorViewController.view;
        
        [self.view addSubview:eventEditorView];
        
        NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(eventEditorView);
        [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[eventEditorView]|"
                                                                          options:0
                                                                          metrics:nil
                                                                            views:viewsDictionary]];
        [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[eventEditorView]|"
                                                                          options:0
                                                                          metrics:nil
                                                                          views:viewsDictionary]];
    }
    
    [self.eventEditorViewController reloadInterface];
    
}


-(void) senderRequestsEventCollisionChainView:(id)sender
{
    if ([self.eventEditorViewController.view superview] == self.view)
    {
        [self.eventEditorViewController.view removeFromSuperview];
    }
}


#pragma mark - NSTableViewDelegate and NSTableViewDataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    BOOST_ASSERT(aTableView == self.eventTableView);
    
    if (aTableView == self.eventTableView)
    {
        EventChain::SPtr chain = self.eventChain.lock();
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
        
        EventChain::SPtr chain = self.eventChain.lock();
        BOOST_ASSERT(chain);
        Event::SPtr event = Event::SPtr();
        if (chain) event = chain->getEventAtIndex((unsigned int)row);
        
        IM::Action::SPtr action = boost::dynamic_pointer_cast<Action>(event);
        if (action)
        {
            VSCIMOSXActionCellView* actionView = [tableView makeViewWithIdentifier:[[VSCIMOSXActionCellView class] description] owner:self];
            BOOST_ASSERT(actionView);
            BOOST_ASSERT([actionView isKindOfClass:[VSCIMOSXActionCellView class]]);
            [actionView setEvent:(Event::WPtr(event))];
            actionView.eventChainController = self;
            if (traceInterface) NSLog(@"Returning: %@ with frame: %@", actionView, NSStringFromRect(actionView.frame));
            [actionView setNeedsLayout:YES];
            return actionView;
        }
        
        IM::Delay::SPtr delay = boost::dynamic_pointer_cast<IM::Delay>(event);
        if (delay)
        {
            VSCIMOSXDelayCellView* delayView = [tableView makeViewWithIdentifier:[[VSCIMOSXDelayCellView class] description] owner:self];
            BOOST_ASSERT(delayView);
            BOOST_ASSERT([delayView isKindOfClass:[VSCIMOSXDelayCellView class]]);
            delayView.eventChainController = self;
            [delayView setEvent:Event::WPtr(event)];
            if (traceInterface) NSLog(@"Returning: %@ with frame: %@", delayView, NSStringFromRect(delayView.frame));
            [delayView setNeedsLayout:YES];
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
        
        EventChain::SPtr chain = self.eventChain.lock();
        BOOST_ASSERT(chain);
        
        if (row >= chain->getNumberOfEvents())
        {
            NSLog(@"ASKING FOR ROW HEIGHT FOR OUT OF BOUNDS INDEX");
            return 0;
        }
        
        Event::SPtr event = Event::SPtr();
        if (chain) event = chain->getEventAtIndex((unsigned int)row);
        
        IM::Action::SPtr action = boost::dynamic_pointer_cast<Action>(event);
        if (action)
        {
            CGFloat h = [VSCIMOSXActionCellView defaultViewHeight];
            if (traceInterface) NSLog(@"Returning: %f", h);
            return h;
        }
        
        IM::Delay::SPtr delay = boost::dynamic_pointer_cast<IM::Delay>(event);
        if (delay)
        {
            CGFloat h = [VSCIMOSXDelayCellView defaultViewHeight];
            if (traceInterface) NSLog(@"Returning: %f", h);
            return h;
        }
        
        BOOST_ASSERT_MSG(false, "Expected Action or Delay");
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
        //NSLog(@"Row %ld: %@ with frame %@", idx, v, NSStringFromRect(v.frame));
        if ([v isKindOfClass:[VSCIMOSXActionCellView class]])
        {
            VSCIMOSXActionCellView* actionView = (VSCIMOSXActionCellView*)v;
            [actionView setNeedsLayout:YES];
            [actionView printUIDescription];
        }
    }];
    
}

- (void)tableViewColumnDidResize:(NSNotification *)aNotification
{
    NSLog(@"%@ tableViewColumnDidResize: %@, column %@, old width %@",
          self, [aNotification object],
          [[aNotification userInfo] objectForKey:@"NSTableColumn"],
          [[aNotification userInfo] objectForKey:@"NSOldWidth"]);
}

- (void)tableView:(NSTableView *)aTableView
  willDisplayCell:(id)aCell
   forTableColumn:(NSTableColumn *)aTableColumn
              row:(NSInteger)rowIndex
{
    NSLog(@"HERE!!");
}


@end

