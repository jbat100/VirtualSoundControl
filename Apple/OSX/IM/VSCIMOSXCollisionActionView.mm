//
//  VSCIMCollisionActionView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionActionView.h"

/*
 *  Private internals
 */

@interface VSCIMOSXCollisionActionView ()

-(void) setupInterfaceForCollisionActionType:(VSCIMOSXCollisionActionType)collisionActionType;

@end


@implementation VSCIMOSXCollisionActionView

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

-(void) setupInterfaceForCollisionActionType:(VSCIMOSXCollisionActionType)collisionActionType
{
    
}

-(IBAction) actionTypeChanged:(id)sender
{
    
}

@end
