//
//  VSCIMOSXCollisionMappingView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionMappingView.h"
#import "VSCIMOSXCollisionMappingGlobal.h"

@interface VSCIMOSXCollisionMappingView ()

@property (nonatomic, assign) VSCIMOSXCollisionMappingType collisionMappingType;

@end

@implementation VSCIMOSXCollisionMappingView


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

-(void) setCollisionMapping:(VSC::IM::CollisionMapping::WPtr)collisionMapping
{
    _collisionMapping = collisionMapping;
    self.collisionMappingType = VSCIMOSXCollisionMappingTypeForCollisionMapping(collisionMapping.lock());
}


@end
