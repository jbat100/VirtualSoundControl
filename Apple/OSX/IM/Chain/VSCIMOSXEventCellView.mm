//
//  VSCIMOSXEventCellView.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 2/3/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCIMOSXEventCellView.h"
#import "VSCIMOSXEventChainController.h"

#include "VSCIMEvent.h"

#include <boost/assert.hpp>

@implementation VSCIMOSXEventCellView

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

-(IBAction) showMappings:(id)sender
{
    BOOST_ASSERT(self.eventChainController);
    BOOST_ASSERT([self.eventChainController respondsToSelector:@selector(sender:requestsShowMappingsForEvent:)]);
    
    [self.eventChainController sender:self requestsShowMappingsForEvent:self.event.lock()];
}

-(void) awakeFromNib
{
    BOOST_ASSERT(self.titleTextField);
    BOOST_ASSERT(self.iconImageView);
    
    self.titleTextField.translatesAutoresizingMaskIntoConstraints = NO;
    self.iconImageView.translatesAutoresizingMaskIntoConstraints = NO;
}

-(void) reloadInterface
{
    
}

#pragma mark - Custon Setter

-(BOOL) checkEvent:(VSC::IM::Event_SPtr)testEvent
{
    return YES;
}

-(void) setEvent:(VSC::IM::Event::WPtr)weakEvent
{
    if (weakEvent.lock() != _event.lock())
    {
        VSC::IM::Event::SPtr event = weakEvent.lock();
        if ([self checkEvent:event])
        {
            _event = event;
        }
        else
        {
            _event = VSC::IM::Event::WPtr();
        }
    }
}

@end
