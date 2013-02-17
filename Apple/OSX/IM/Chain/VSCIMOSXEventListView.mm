//
//  VSCIMOSXEventListView.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 2/17/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCIMOSXEventListView.h"

#include <boost/assert.hpp>

@implementation VSCIMOSXEventListView

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
    BOOST_ASSERT(self.eventTableView);
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

@end
