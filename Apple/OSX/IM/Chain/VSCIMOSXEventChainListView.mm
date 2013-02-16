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

@end
