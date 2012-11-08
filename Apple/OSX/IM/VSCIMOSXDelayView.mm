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

-(IBAction) delayChanged:(id)sender;

@end

@implementation VSCIMOSXDelayView

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
    [(NSNumberFormatter*)[self.delayTextField formatter] setThousandSeparator:@""];
}

-(IBAction) delayChanged:(id)sender
{
    
}

@end
