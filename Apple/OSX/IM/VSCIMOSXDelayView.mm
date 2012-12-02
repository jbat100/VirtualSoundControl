//
//  VSCIMOSXDelayView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/30/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXDelayView.h"

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
    if (self) {
        [self commonInit];
    }
    return self;
}

-(void) commonInit
{
    //self.translatesAutoresizingMaskIntoConstraints = NO;
}

- (void)drawRect:(NSRect)dirtyRect
{
    CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    
    CGContextSetRGBFillColor (myContext, 0.0, 0.0, 1.0, 1.0);
    CGContextFillRect(myContext, self.bounds);
    
    CGContextSetGrayStrokeColor (myContext, 1.0, 1.0);
    CGContextStrokeRectWithWidth(myContext, NSRectToCGRect(self.bounds), 2.0);
}

-(void) awakeFromNib
{
    [(NSNumberFormatter*)[self.delayTextField formatter] setThousandSeparator:@""];
}

-(IBAction) delayChanged:(id)sender
{
    
}

@end
