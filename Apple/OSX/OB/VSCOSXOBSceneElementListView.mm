//
//  VSCOSXOBSceneElementListView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementListView.h"

#include <boost/assert.hpp>

@implementation VSCOSXOBSceneElementListView

-(void) dealoc
{
    NSLog(@"%@ DEALLOC", self);
}

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
    BOOST_ASSERT(self.elementTableView);
    BOOST_ASSERT(self.elementTableView.delegate);
    BOOST_ASSERT(self.elementTableView.dataSource);
    
    NSLog(@"%@ awakeFromNib, elementTableView: %@, self.elementTableView.delegate: %@",
          self, self.elementTableView, self.elementTableView.delegate);
}

@end
