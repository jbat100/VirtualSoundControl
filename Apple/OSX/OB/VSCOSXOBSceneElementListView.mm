//
//  VSCOSXOBSceneElementListView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementListView.h"
#import "PXListView.h"

#include <boost/assert.hpp>

@implementation VSCOSXOBSceneElementListView

@synthesize listView = _listView;

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
    BOOST_ASSERT(self.listView);
    BOOST_ASSERT(self.listView.delegate);
    
    NSLog(@"%@ awakeFromNib, self.listView: %@, self.listView.delegate: %@", self, self.listView, self.listView.delegate);
}

@end
