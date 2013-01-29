//
//  VSCIMOSXDelayView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/30/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXDelayView.h"
#import "VSCIMOSXEventChainController.h"

#include "VSC.h"
#include "VSCIMDelay.h"

#include <boost/assert.hpp>

@interface VSCIMOSXDelayView ()

@property (weak) IBOutlet NSTextField* titleTextField;
@property (weak) IBOutlet NSTextField* delayTextField;

-(void) commonInit;

-(IBAction) delayChanged:(id)sender;

@end

@implementation VSCIMOSXDelayView

+(CGFloat) defaultViewHeight
{
    return 40.0;
}

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self commonInit];
    }
    return self;
}

-(id) initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self)
    {
        [self commonInit];
    }
    return self;
}

-(void) commonInit
{
    self.translatesAutoresizingMaskIntoConstraints = NO;
}

- (void)drawRect:(NSRect)dirtyRect
{
    CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    CGContextSetRGBFillColor (myContext, 0.4, 0.4, 0.4, 1.0);
    CGContextFillRect(myContext, self.bounds);
    CGContextSetGrayStrokeColor (myContext, 1.0, 1.0);
    CGContextStrokeRectWithWidth(myContext, NSRectToCGRect(self.bounds), 2.0);
}

-(void) awakeFromNib
{
    [(NSNumberFormatter*)[self.delayTextField formatter] setThousandSeparator:@""];
}

#pragma mark - Custom Setters

-(void) setDelay:(VSC::IM::Delay::WPtr)delay
{
    if (delay.lock() != _delay.lock())
    {
        _delay = delay;
        [self reloadInterface];
    }
}

#pragma mark - UI Helpers

-(void) reloadInterface
{
    VSC::IM::Delay::SPtr d = self.delay.lock();
    if (d)
    {
        VSC::TimeDuration duration = d->getDelay();
        double milliseconds = (double)duration.total_milliseconds();
        double seconds = milliseconds/1000.0;
        [self.delayTextField setDoubleValue:seconds];
    }
    else
    {
        [self.delayTextField setStringValue:@"-- EMPTY --"];
    }
}

#pragma mark - UI Callbacks

-(IBAction) delayChanged:(id)sender
{
    BOOST_ASSERT(self.eventChainController);
    BOOST_ASSERT([self.eventChainController respondsToSelector:@selector(sender:requestsSetDelay:toInterval:)]);
    
    if ([self.eventChainController respondsToSelector:@selector(sender:requestsSetDelay:toInterval:)])
    {
        BOOST_ASSERT(self.delayTextField);
        BOOST_ASSERT(self.delayTextField == sender);
        NSTimeInterval delayInterval = (NSTimeInterval)[self.delayTextField doubleValue];
        [self.eventChainController sender:self requestsSetDelay:self.delay.lock() toInterval:delayInterval];
    }
}

@end
