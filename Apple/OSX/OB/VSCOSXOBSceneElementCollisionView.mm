//
//  VSCOSXOBSceneElementCollisionEventChainView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementCollisionView.h"
#import "VSCOSXOBSceneElementController.h"
#import "VSCIMOSXCollisionEventChainEditor.h"
#import "VSCIMOSXCollisionEventChainView.h"

#include <boost/assert.hpp>

@interface VSCOSXOBSceneElementCollisionView ()

-(void) customInit;

@end

@implementation VSCOSXOBSceneElementCollisionView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self customInit];
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        [self customInit];
    }
    return self;
}

-(void) customInit
{
    
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void) awakeFromNib
{
    self.translatesAutoresizingMaskIntoConstraints = NO;
    
    [self.collisionStartedEventChainView senderRequestsEventChainView:self];
    [self.collisionEndedEventChainView senderRequestsEventChainView:self];
    
}

-(void) reloadInterface
{
    [self.collisionStartedEventChainView reloadInterface];
    [self.collisionEndedEventChainView reloadInterface];
}

@end
