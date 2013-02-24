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
    
    BOOST_ASSERT(self.appendEventButton);
    BOOST_ASSERT(self.removeEventButton);
    BOOST_ASSERT(self.addEventMenu);
    
    self.appendEventButton.translatesAutoresizingMaskIntoConstraints = NO;
    self.removeEventButton.translatesAutoresizingMaskIntoConstraints = NO;
    
    BOOST_ASSERT(self.appendDelayMenuItem);
    BOOST_ASSERT(self.appendActionMenuItem);
    BOOST_ASSERT(self.insertDelayMenuItem);
    BOOST_ASSERT(self.insertActionMenuItem);
    
    self.eventTableView.allowsEmptySelection = YES;
    self.eventTableView.allowsMultipleSelection = NO;
}

@end
