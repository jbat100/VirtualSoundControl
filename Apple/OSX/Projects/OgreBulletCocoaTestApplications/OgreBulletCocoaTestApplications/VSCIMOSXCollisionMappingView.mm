//
//  VSCIMOSXCollisionMappingView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionMappingView.h"

@interface VSCIMOSXCollisionMappingView ()

+(VSCIMOSXCollisionMappingType) typeOfCollisionMapping:(VSC::IM::CollisionMapping::SPtr)collisionMapping;

@property (nonatomic, assign) VSCIMOSXCollisionMappingType collisionMappingType;

@end

@implementation VSCIMOSXCollisionMappingView

+(VSCIMOSXCollisionMappingType) typeOfCollisionMapping:(VSC::IM::CollisionMapping::SPtr)collisionMapping
{
    if (collisionMapping)
    {
        VSC::IM::CollisionConstantMapping::SPtr constant = boost::dynamic_pointer_cast<VSC::IM::CollisionConstantMapping>(collisionMapping);
        if (constant) return VSCIMOSXCollisionMappingTypeConstant;
        
        VSC::IM::CollisionVelocityMapping::SPtr velocity = boost::dynamic_pointer_cast<VSC::IM::CollisionVelocityMapping>(collisionMapping);
        if (velocity) return VSCIMOSXCollisionMappingTypeVelocity;
        
        VSC::IM::CollisionDistanceMapping::SPtr distance = boost::dynamic_pointer_cast<VSC::IM::CollisionDistanceMapping>(collisionMapping);
        if (distance) return VSCIMOSXCollisionMappingTypeDistance;
    }
    
    return VSCIMOSXCollisionMappingTypeNone;
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

-(void) setCollisionMapping:(VSC::IM::CollisionMapping::WPtr)collisionMapping
{
    _collisionMapping = collisionMapping;
    
    self.collisionMappingType = [VSCIMOSXCollisionMappingView typeOfCollisionMapping:collisionMapping.lock()];
    
}


@end
