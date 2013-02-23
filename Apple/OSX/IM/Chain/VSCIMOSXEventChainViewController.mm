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
#import "VSCIMOSXEventListView.h"
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
-(void) insertEvent:(Event::SPtr)event;
-(void) removeEvent:(Event::SPtr)event;

-(void) updateAfterTableViewSelectionChange;

@end

@implementation VSCIMOSXEventChainViewController

const static BOOL traceInterface = YES;

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
    
    BOOST_ASSERT(self.eventListView.eventTableView.delegate == self);
    BOOST_ASSERT(self.eventListView.eventTableView.dataSource == self);
}

-(void) setEventChain:(EventChain::WPtr)eventChain
{
    _eventChain = eventChain;
    
    [self reloadInterface];
    
    [self updateAfterTableViewSelectionChange];
}

-(void) reloadInterface
{
    [self.eventListView.eventTableView reloadData];
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
    NSInteger index = [self.eventListView.eventTableView selectedRow];
    
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
    
    if (sender == self.eventListView.appendDelayMenuItem)
    {
        event = Event::SPtr(new IM::Delay);
        [self appendEvent:event];
    }
    else if (sender == self.eventListView.appendActionMenuItem)
    {
        event = Event::SPtr(new Action);
        Action::SPtr action = boost::dynamic_pointer_cast<Action>(event);
        action->setActionType(ActionTypeVoid);
        [self appendEvent:event];
    }
    else if (sender == self.eventListView.insertDelayMenuItem)
    {
        event = Event::SPtr(new IM::Delay);
        [self insertEvent:event];
    }
    else if (sender == self.eventListView.insertActionMenuItem)
    {
        event = Event::SPtr(new Action);
        Action::SPtr action = boost::dynamic_pointer_cast<Action>(event);
        action->setActionType(ActionTypeVoid);
        [self insertEvent:event];
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
        [self updateAfterTableViewSelectionChange];
    }
}

-(void) insertEvent:(Event::SPtr)event
{
    Event::SPtr selectedEvent = [self selectedChainEvent];
    EventChain::SPtr chain = self.eventChain.lock();
    
    if (selectedEvent && chain)
    {
        BOOST_ASSERT(chain);
        BOOST_ASSERT(event);
        
        chain->insertEventBeforeEvent(event, selectedEvent);
        [self reloadInterface];
        [self updateAfterTableViewSelectionChange];
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
        [self updateAfterTableViewSelectionChange];
    }
    
}

#pragma mark - UI Helpers

-(void) resetView
{
    [[self.view subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
}

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
        
        [self.eventEditorViewController view].translatesAutoresizingMaskIntoConstraints = NO;
    }
    
    BOOST_ASSERT(event);
    
    self.eventEditorViewController.event = Event::WPtr(event);
    
    [self switchToView:[self.eventEditorViewController view]];
    
    //[self.eventEditorViewController reloadInterface];
    
}


-(void) senderRequestsEventChainView:(id)sender
{
    [self switchToView:self.eventListView];
    
    [self.eventListView.eventTableView reloadData];
    
    [[self view] setNeedsLayout:YES];
}

-(void) updateAfterTableViewSelectionChange
{
    NSIndexSet* rowIndexes = [self.eventListView.eventTableView selectedRowIndexes];
    
    if ([rowIndexes count] > 0)
    {
        [self.eventListView.appendEventButton setEnabled:YES];
        [self.eventListView.insertEventButton setEnabled:YES];
        [self.eventListView.removeEventButton setEnabled:YES];
    }
    else
    {
        [self.eventListView.appendEventButton setEnabled:YES];
        [self.eventListView.insertEventButton setEnabled:NO];
        [self.eventListView.removeEventButton setEnabled:NO];
    }
}

#pragma mark - NSTableViewDelegate and NSTableViewDataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    BOOST_ASSERT(aTableView == self.eventListView.eventTableView);
    
    if (aTableView == self.eventListView.eventTableView)
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
    BOOST_ASSERT(tableView == self.eventListView.eventTableView);
    
    static NSDictionary* metrics = @{@"actionHeight" : [NSNumber numberWithFloat:[VSCIMOSXActionCellView defaultViewHeight]],
                                     @"delayHeight" : [NSNumber numberWithFloat:[VSCIMOSXDelayCellView defaultViewHeight]]};
    
    if (tableView == self.eventListView.eventTableView)
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
            
            /*
            
            NSView* mainView = [self view];
            
            NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(actionView, mainView);
            NSArray* hConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:[actionView(==mainView)]"
                                                                            options:0 metrics:metrics views:viewsDictionary];
            [actionView addConstraints:hConstraints];
            NSArray* vConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"V:[actionView(>=actionHeight)]"
                                                                            options:0 metrics:metrics views:viewsDictionary];
            [actionView addConstraints:vConstraints];
            
            */
            
            BOOST_ASSERT(actionView);
            BOOST_ASSERT([actionView isKindOfClass:[VSCIMOSXActionCellView class]]);
            
            actionView.selected = NO;
            
            //actionView.translatesAutoresizingMaskIntoConstraints = NO;
            
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
            
            delayView.selected = NO;
            
            delayView.translatesAutoresizingMaskIntoConstraints = NO;
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
    
    BOOST_ASSERT(tableView == self.eventListView.eventTableView);
    
    if (tableView == self.eventListView.eventTableView)
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
    
    BOOST_ASSERT([aNotification object] == self.eventListView.eventTableView);
    if ([aNotification object] != self.eventListView.eventTableView) return;
    
    /*
     *  Deselect all views
     */
    
    [self.eventListView.eventTableView enumerateAvailableRowViewsUsingBlock:^(NSTableRowView *rowView, NSInteger row) {
        NSView* v = [rowView viewAtColumn:0];
        BOOST_ASSERT([v isKindOfClass:[VSCIMOSXEventCellView class]]);
        if ([v isKindOfClass:[VSCIMOSXEventCellView class]])
        {
            VSCIMOSXEventCellView* eventView = (VSCIMOSXEventCellView*)v;
            eventView.selected = NO;
        }
    }];
    
    /*
     *  (Re)select the ones
     */

    NSIndexSet* rowIndexes = [self.eventListView.eventTableView selectedRowIndexes];

    [rowIndexes enumerateIndexesUsingBlock:^(NSUInteger idx, BOOL *stop) {
        
        NSTableRowView* rowView = [self.eventListView.eventTableView rowViewAtRow:idx makeIfNecessary:NO];
        
        NSLog(@"%@ frame %@", rowView, NSStringFromRect(rowView.frame));
        
        NSView* v = [self.eventListView.eventTableView viewAtColumn:0 row:idx makeIfNecessary:NO];
        if ([v isKindOfClass:[VSCIMOSXEventCellView class]])
        {
            VSCIMOSXEventCellView* eventView = (VSCIMOSXEventCellView*)v;
            eventView.selected = YES;
            [eventView printUIDescription];
        }
    }];
    
    [self updateAfterTableViewSelectionChange];
    
}

- (void)tableViewColumnDidResize:(NSNotification *)aNotification
{

}

-(void)tableView:(NSTableView *)tableView didAddRowView:(NSTableRowView *)rowView forRow:(NSInteger)row
{
    [rowView setNeedsLayout:YES];
    
    NSView* cellView = [rowView viewAtColumn:0];
    
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(cellView);
    
    NSArray* hConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[cellView]|"
                                                                    options:0 metrics:nil views:viewsDictionary];
    [[cellView superview] addConstraints:hConstraints];
    
    NSArray* vConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[cellView]|"
                                                                    options:0 metrics:nil views:viewsDictionary];
    [[cellView superview] addConstraints:vConstraints];
    
    [cellView setNeedsLayout:YES];
    [rowView setNeedsLayout:YES];
}

- (BOOL)tableView:(NSTableView *)aTableView shouldSelectRow:(NSInteger)rowIndex
{
    NSLog(@"%@ tableView: %@ shouldSelectRow: %ld", self, aTableView, (long)rowIndex);
    return YES;
}

@end

