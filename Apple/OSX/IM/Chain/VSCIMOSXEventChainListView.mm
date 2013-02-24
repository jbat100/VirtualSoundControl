//
//  VSCIMOSXEventChainListView.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/30/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCIMOSXEventChainListView.h"
#import "VSCOSXEnvironmentController.h"
#import "VSCIMOSXEventChainCellView.h"

#include "VSCEnvironment.h"
#include "VSCIM.h"

#include <boost/assert.hpp>

using namespace VSC;
using namespace VSC::IM;

@interface VSCIMOSXEventChainListView ()

-(EventChain::SPtr) selectedEventChain;

@end

@implementation VSCIMOSXEventChainListView

static const bool traceInterface = true;

#pragma mark - NSView Methods

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

-(void) awakeFromNib
{
    self.tableView.translatesAutoresizingMaskIntoConstraints = NO;
    self.addButton.translatesAutoresizingMaskIntoConstraints = NO;
    self.removeButton.translatesAutoresizingMaskIntoConstraints = NO;
}

-(EventChain::SPtr) selectedEventChain
{
    BOOST_ASSERT(self.tableView);
    if (!self.tableView) return EventChain::SPtr();
    NSIndexSet* selectedIndexSet = [self.tableView selectedRowIndexes];
    if ([selectedIndexSet count] == 0) return EventChain::SPtr();
    BOOST_ASSERT([selectedIndexSet count] == 1);
    if ([selectedIndexSet count] != 1) return EventChain::SPtr();
    NSUInteger index = [selectedIndexSet firstIndex];
    
    BOOST_ASSERT(self.environmentController);
    if (!self.environmentController) return EventChain::SPtr();
    Environment::SPtr environment = self.environmentController.environment.lock();
    BOOST_ASSERT(environment);
    if (!environment) return EventChain::SPtr();
    const EventChains& eventChains = environment->getEventChains();
    BOOST_ASSERT(eventChains.size() > index);
    if (eventChains.size() > index) return eventChains.at(index);
    return EventChain::SPtr();
}

-(void) reloadInterface
{
    [self.tableView reloadData];
}

@end
