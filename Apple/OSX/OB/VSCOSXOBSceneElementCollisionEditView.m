//
//  VSCOSXOBSceneElementCollisionEditView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/24/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementCollisionEditView.h"

@implementation VSCOSXOBSceneElementCollisionEditView

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


-(void) showCollisionEventChain
{
    BOOST_ASSERT(self.mainView);
    if(!self.mainView) return;
    
    if (!self.collisionEventChainView)
    {
        NSArray* topLevelObjects = nil;
        
        [[NSBundle mainBundle] loadNibNamed:@"VSCIMOSXCollisionEventChainView"
                                      owner:self
                            topLevelObjects:&topLevelObjects];
        
        /*
         *  Just loading the nib should set the self.collisionEventChainView of the owner (self)
         */
        
        BOOST_ASSERT(self.collisionEventChainView);
        BOOST_ASSERT(self.collisionEventChainView.eventChainController == self);
        
        [self.collisionEventChainView setAutoresizingMask:(NSUInteger)(NSViewWidthSizable | NSViewHeightSizable)];
        [self.collisionEventChainView setAutoresizesSubviews:YES];
    }
    
    [self.collisionMappingListView removeFromSuperview];
    
    if ([self.collisionEventChainView superview] != self.mainView)
    {
        [self.mainView addSubview:self.collisionEventChainView];
    }
    
    self.collisionEventChainView.frame = self.mainView.bounds;
    
    
}

-(void) showCollisionMappingsForCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction
{
    BOOST_ASSERT(self.mainView);
    if(!self.mainView) return;
    
    if (!self.collisionMappingListView)
    {
        NSArray* topLevelObjects = nil;
        
        [[NSBundle mainBundle] loadNibNamed:@"VSCIMOSXCollisionMappingListView"
                                      owner:self
                            topLevelObjects:&topLevelObjects];
        
        BOOST_ASSERT(self.collisionMappingListView);
        BOOST_ASSERT(self.collisionMappingListView.eventChainController == self);
        
        [self.collisionMappingListView setAutoresizingMask:(NSUInteger)(NSViewWidthSizable | NSViewHeightSizable)];
        [self.collisionMappingListView setAutoresizesSubviews:YES];
    }
    
    [self.collisionEventChainView removeFromSuperview];
    
    if ([self.collisionMappingListView superview] != self.mainView)
    {
        [self.mainView addSubview:self.collisionEventChainView];
    }
    
    self.collisionEventChainView.frame = self.mainView.bounds;
}



@end
