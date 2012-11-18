//
//  VSCOSXOBSceneElementInspectorView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementInspectorView.h"

#include <boost/assert.hpp>

NSDictionary* elementInspectorTabDict = nil;

@interface VSCOSXOBSceneElementInspectorView ()

@property (weak) IBOutlet DMTabBar* tabBar;
@property (weak) IBOutlet NSView* mainView;

@end

@implementation VSCOSXOBSceneElementInspectorView

+ (void)load
{
    
}

- (void)awakeFromNib
{
    NSLog(@"%@ awakeFromNib", self);
    
    //BOOST_ASSERT(self.elementDetailView);
    //BOOST_ASSERT(self.elementCollisionView);
    
}

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

-(void) showElementDetailView
{
    if (!self.elementDetailView)
    {
        NSArray* topLevelObjects = nil;
        
        [[NSBundle mainBundle] loadNibNamed:@"VSCOSXOBSceneElementDetailView"
                                      owner:self
                            topLevelObjects:&topLevelObjects];
        
        BOOST_ASSERT(self.elementDetailView);
    }
    
    [self.elementCollisionView setHidden:YES];
    if ([self.elementDetailView superview] != self.mainView)
    {
        [self.mainView addSubview:self.elementDetailView];
    }
    self.elementDetailView.frame = self.mainView.bounds;
    [self.elementDetailView setHidden:NO];
}

-(void) showElementCollisionView
{
    if (!self.elementCollisionView)
    {
        NSArray* topLevelObjects = nil;
        
        [[NSBundle mainBundle] loadNibNamed:@"VSCOSXOBSceneElementCollisionView"
                                      owner:self
                            topLevelObjects:&topLevelObjects];
        
        BOOST_ASSERT(self.elementDetailView);
    }
    
    [self.elementDetailView setHidden:YES];
    if ([self.elementCollisionView superview] != self.mainView)
    {
        [self.mainView addSubview:self.elementCollisionView];
    }
    self.elementCollisionView.frame = self.mainView.bounds;
    [self.elementCollisionView setHidden:NO];
}

@end
