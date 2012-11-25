//
//  VSCOSXOBSceneElementCollisionEventChainView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementCollisionView.h"
#import "VSCOSXOBSceneElementController.h"
#import "VSCIMOSXCollisionEventChainView.h"
#import "VSCIMOSXCollisionEventChainEditView.h"

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
    
    NSArray* topLevelObjects = nil;
    
    [[NSBundle mainBundle] loadNibNamed:@"VSCIMOSXCollisionEventChainView" owner:nil topLevelObjects:&topLevelObjects];
    
    for (NSObject* topLevelObject in topLevelObjects)
    {
        if ([topLevelObject isKindOfClass:[VSCIMOSXCollisionEventChainEditView class]])
        {
            self.collisionStartedEditView = (VSCIMOSXCollisionEventChainEditView*)topLevelObjects;
        }
    }
    
    BOOST_ASSERT(self.collisionStartedContainerView);
    BOOST_ASSERT(self.collisionStartedEditView);
    
    if (self.collisionStartedContainerView && self.collisionStartedEditView)
    {
        self.collisionStartedEditView.frame = self.collisionStartedContainerView.bounds;
        [self.collisionStartedContainerView addSubview:self.collisionStartedEditView];
        
        NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(_collisionStartedEditView);
        NSArray *hConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_collisionStartedEditView]|"
                                                                        options:0
                                                                        metrics:nil
                                                                          views:viewsDictionary];
        NSArray *vConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[_collisionStartedEditView]|"
                                                                        options:0
                                                                        metrics:nil
                                                                          views:viewsDictionary];
        
        [self.collisionStartedContainerView addConstraints:hConstraints];
        [self.collisionStartedContainerView addConstraints:vConstraints];
    }
    
    [self.collisionStartedEditView showEventChainView];
    
    
}

-(void) reloadInterface
{
    [self.collisionStartedEditView reloadInterface];
    [self.collisionEndedEditView reloadInterface];
}

@end
