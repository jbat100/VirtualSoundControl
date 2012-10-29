//
//  VSCOSXOBSceneElementInspectorView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementInspectorView.h"

NSDictionary* elementInspectorTabDict = nil;

@implementation VSCOSXOBSceneElementInspectorView

+ (void)load
{
    
    
    
}

- (void)awakeFromNib
{
    NSLog(@"%@ awakeFromNib", self);
    
    BOOST_ASSERT(self.elementDetailView);
    BOOST_ASSERT(self.elementCollisionView);
    
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

@end
