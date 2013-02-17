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

+(CGFloat) defaultViewHeight
{
    return 40.0;
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
    //BOOST_ASSERT(self.iconImageView);
    BOOST_ASSERT(self.editorButton);
    
    self.iconImageView.translatesAutoresizingMaskIntoConstraints = NO;
    self.editorButton.translatesAutoresizingMaskIntoConstraints = NO;
}

#pragma mark - UI Callbacks

-(IBAction) showEditor:(id)sender
{
    BOOST_ASSERT(self.eventChainController);
    BOOST_ASSERT([self.eventChainController respondsToSelector:@selector(sender:requestsShowEventEditorForEvent:)]);
    
    [self.eventChainController sender:self requestsShowEventEditorForEvent:self.event.lock()];
}

#pragma mark - Custom Setter

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

#pragma mark - UI Debugging

- (void)printUIDescription
{
    NSLog(@"------------------%@ printUIDescription----------------------", self);
    NSLog(@"frame %@, bounds %@", NSStringFromRect(self.frame), NSStringFromRect(self.bounds));
    NSLog(@"-------------------------------------------------------------");
}

@end
