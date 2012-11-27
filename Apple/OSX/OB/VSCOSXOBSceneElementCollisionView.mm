//
//  VSCOSXOBSceneElementCollisionEventChainView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementCollisionView.h"
#import "VSCOSXEnvironmentController.h"
#import "VSCOSXOBSceneElementController.h"
#import "VSCIMOSXCollisionEventChainController.h"
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
    
    self.collisionStartedEventChainView.elementController = self.elementController;
    
    [self.collisionStartedEventChainView senderRequestsEventChainView:self];
    //[self.collisionEndedEventChainView senderRequestsEventChainView:self];
    
    NSArray* topLevelObjects = nil;
    [[NSBundle mainBundle] loadNibNamed:@"VSCIMOSXCollisionEventChainView"
                                  owner:nil
                        topLevelObjects:&topLevelObjects];
    
    for (NSObject* topLevelObject in topLevelObjects)
    {
        if ([topLevelObject isKindOfClass:[VSCIMOSXCollisionEventChainView class]])
        {
            self.collisionStartedEventChainView = (VSCIMOSXCollisionEventChainView*)topLevelObject;
        }
    }
    
    BOOST_ASSERT(self.collisionStartedContainerView);
    BOOST_ASSERT(self.collisionStartedEventChainView);
    
    [self.collisionStartedContainerView addSubview:self.collisionStartedEventChainView];
    
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(_collisionStartedEventChainView);
    NSArray* hConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_collisionStartedEventChainView]|"
                                                                    options:0
                                                                    metrics:nil
                                                                      views:viewsDictionary];
    NSArray* vConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[_collisionStartedEventChainView]|"
                                                                    options:0
                                                                    metrics:nil
                                                                      views:viewsDictionary];
    [self.collisionStartedContainerView addConstraints:hConstraints];
    [self.collisionStartedContainerView addConstraints:vConstraints];
    
    [self reloadInterface];
    
}

#pragma mark - Custom Setters

-(void) setElementController:(id<VSCOSXOBSceneElementController>)elementController
{
    _elementController = elementController;
    
    self.collisionStartedEventChainView.elementController = elementController;
    self.collisionEndedEventChainView.elementController = elementController;
    
    [self reloadInterface];
}

#pragma mark - UI Helpers

-(void) reloadInterface
{
    //BOOST_ASSERT(self.elementController);
    
    VSC::IM::CollisionEventChain::SPtr startedChain = VSC::IM::CollisionEventChain::SPtr();
    
    if (self.elementController)
    {
        VSC::OB::Scene::Element::SPtr element = self.elementController.element.lock();
        BOOST_ASSERT(element);
        
        if (element && self.elementController.environmentController)
        {
            startedChain = [self.elementController.environmentController collisionStartedEventChainForElement:element];
        }
    }
    
    [self.collisionStartedEventChainView setEventChain:VSC::IM::CollisionEventChain::WPtr(startedChain)];
    
    [self.collisionStartedEventChainView reloadInterface];
    //[self.collisionEndedEventChainView reloadInterface];
}

@end
