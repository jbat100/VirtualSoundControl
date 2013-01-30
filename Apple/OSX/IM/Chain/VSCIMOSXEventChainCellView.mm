//
//  VSCIMOSXEventChainCellView.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/30/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCIMOSXEventChainCellView.h"
#import "NSString+VSCAdditions.h"

#include <boost/assert.hpp>

@implementation VSCIMOSXEventChainCellView

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
    BOOST_ASSERT(self.labelTextField);
    BOOST_ASSERT(self.editButton)
}

-(void) setEventChain:(VSC::IM::EventChain::WPtr)chain
{
    VSC::IM::EventChain::SPtr newChain = chain.lock();
    BOOST_ASSERT(newChain);
    
    if (newChain != _eventChain.lock())
    {
        _eventChain = chain;
        if (newChain)
        {
            [self.labelTextField setStringValue:[NSString stringWithStdString:newChain->getUsername()]];
        }
        else
        {
            [self.labelTextField setStringValue:@"__EMPTY__"];
        }
    }
}

@end
