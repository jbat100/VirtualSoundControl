//
//  VSCIMOSXDelayCellView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/30/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXDelayCellView.h"
#import "VSCIMOSXEventChainController.h"

#include "VSC.h"
#include "VSCIMDelay.h"

#include <boost/assert.hpp>

@interface VSCIMOSXDelayCellView ()

-(void) commonInit;

-(IBAction) delayChanged:(id)sender;

@end

@implementation VSCIMOSXDelayCellView

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
    [super awakeFromNib];
    
    BOOST_ASSERT(self.titleTextField);
    self.titleTextField.translatesAutoresizingMaskIntoConstraints = NO;
    self.translatesAutoresizingMaskIntoConstraints = NO;
}

#pragma mark - Delay Getter

-(VSC::IM::Delay::SPtr) delay
{
    VSC::IM::Event::SPtr delayEvent = self.event.lock();
    BOOST_ASSERT(delayEvent);
    VSC::IM::Delay::SPtr delay = boost::dynamic_pointer_cast<VSC::IM::Delay>(delayEvent);
    BOOST_ASSERT(delay);
    return delay;
}

#pragma mark - Custom Setters

-(BOOL) checkEvent:(VSC::IM::Event::SPtr)testEvent
{
    BOOST_ASSERT(testEvent);
    if (!testEvent) return YES;
    VSC::IM::Delay::SPtr delay = boost::dynamic_pointer_cast<VSC::IM::Delay>(testEvent);
    BOOST_ASSERT(delay);
    if (delay) return YES;
    return NO;
}


@end
