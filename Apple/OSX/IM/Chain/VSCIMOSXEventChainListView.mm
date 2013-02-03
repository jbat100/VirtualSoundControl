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

#include <boost/assert.hpp>

@interface VSCIMOSXEventChainListView ()

+(VSCIMOSXEventChainCellView*) newEventChainCellViewWithOwner:(id)owner;

@end

@implementation VSCIMOSXEventChainListView

static const bool traceInterface = true;

#pragma mark - NSView Static Factory

+(VSCIMOSXEventChainCellView*) newEventChainCellViewWithOwner:(id)owner
{
    static NSNib* nib = nil;
    static NSString* identifier = [[VSCIMOSXEventChainCellView class] description];
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        BOOST_ASSERT(!nib);
        nib = [[NSNib alloc] initWithNibNamed:[[VSCIMOSXEventChainCellView class] description] bundle:nil];
    });
    BOOST_ASSERT(nib);
    
    NSArray *objects = nil;
    VSCIMOSXEventChainCellView* v = nil;
    [nib instantiateNibWithOwner:owner topLevelObjects:&objects];
    for(id object in objects)
    {
        if([object isKindOfClass:[VSCIMOSXEventChainCellView class]]) {
            v = object;
            v.identifier = identifier;
            break;
        }
    }
    BOOST_ASSERT(v);
    BOOST_ASSERT(v.environmentController == owner);
    v.translatesAutoresizingMaskIntoConstraints = NO;
    return v;
}

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

#pragma mark - NSTableView Delegate and DataSource Methods


- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    BOOST_ASSERT(aTableView == self.tableView);
    BOOST_ASSERT(self.environmentController);
    
    if (aTableView == self.tableView && self.environmentController)
    {
        VSC::Environment::SPtr environment = self.environmentController.environment.lock();
        
        BOOST_ASSERT(environment);
        
        if (environment)
        {
            const VSC::IM::EventChains& chains = environment->getEventChains();
            return chains.size();
        }
        
    }
    
	return 0;
}

- (NSView *)tableView:(NSTableView *)aTableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row
{
    BOOST_ASSERT(aTableView == self.tableView);
    BOOST_ASSERT(self.environmentController);
    
    if (aTableView == self.tableView && self.environmentController)
    {
        if (traceInterface) NSLog(@"%@ listView:cellForRow: %ld", self, row);
        
        VSC::Environment::SPtr environment = self.environmentController.environment.lock();
        BOOST_ASSERT(environment);
        if (environment)
        {
            const VSC::IM::EventChains& chains = environment->getEventChains();
            VSC::IM::EventChain::SPtr chain = chains.at(row);
            BOOST_ASSERT(chain);
            if (chain)
            {
                VSCIMOSXEventChainCellView* view = nil;
                view = [aTableView makeViewWithIdentifier:[[VSCIMOSXEventChainCellView class] description] owner:self];
                if (view) BOOST_ASSERT([view isKindOfClass:[VSCIMOSXEventChainCellView class]]);
                else view = [[self class] newEventChainCellViewWithOwner:self];
                [view setEventChain:VSC::IM::EventChain::WPtr(chain)];
                return view;
            }
        }
    }
    
	return nil;
}

- (CGFloat)tableView:(NSTableView *)aTableView heightOfRow:(NSInteger)row
{
    BOOST_ASSERT(aTableView == self.tableView);
    if (aTableView == self.tableView)
    {
        [VSCIMOSXEventChainCellView defaultViewHeight];
    }
    return 0;
}

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
    NSLog(@"%@ tableViewSelectionDidChange: %@", self, aNotification);
}


@end
