//
//  VSCIMOSXCollisionEventChainView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/29/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionEventChainView.h"
#import "PXListView.h"
#import "PXListViewCell.h"

#include "VSCIMDelay.h"

#include <boost/assert.hpp>

@interface VSCIMOSXCollisionEventChainView ()


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

-(VSC::IM::Event::SPtr) selectedChainEvent
{
    NSIndexSet* indexSet = [self.collisionEventListView selectedRows];
    
    BOOST_ASSERT([indexSet count] <= 1);
    
    if ([indexSet count] == 1)
    {
        NSUInteger index = [indexSet firstIndex];
        
        if (self.eventChainController)
        {
            VSC::IM::CollisionEventChain::SPtr chain = self.eventChainController.collisionEventChain.lock();
            if (chain) return chain->getEvents().at(index);
        }
    }
    
    return VSC::IM::Event::SPtr();
}


@end
