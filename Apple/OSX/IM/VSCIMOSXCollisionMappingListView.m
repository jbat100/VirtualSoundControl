//
//  VSCIMOSXCollisionMappingListView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/8/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionMappingListView.h"

@interface VSCIMOSXCollisionMappingListView ()

@property (weak) IBOutlet NSButton* backToEventChainViewButton;

-(IBAction) backToEventChainView:(id)sender;

@end

@implementation VSCIMOSXCollisionMappingListView

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

-(IBAction) backToEventChainView:(id)sender
{
    [self.eventChainController showCollisionEventChain];
}

@end
