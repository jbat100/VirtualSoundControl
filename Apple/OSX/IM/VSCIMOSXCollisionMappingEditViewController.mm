//
//  VSCIMOSXCollisionMappingEditViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionMappingEditViewController.h"

#include <boost/assert.hpp>

@interface VSCIMOSXCollisionMappingEditViewController ()

-(void) reloadInterface;

@end

@implementation VSCIMOSXCollisionMappingEditViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        // Initialization code here.
    }
    
    return self;
}

-(void) setCollisionMapping:(VSC::IM::CollisionMapping::WPtr)collisionMapping
{
    if (collisionMapping.lock() != _collisionMapping.lock())
    {
        _collisionMapping = collisionMapping;
        [self reloadInterface];
    }
}

-(void) reloadInterface
{
    VSC::IM::CollisionMapping::SPtr mapping = _collisionMapping.lock();
    BOOST_ASSERT(mapping);
    if (mapping)
    {
        self.scaleFactor = mapping->getScaleFactor();
        self.offset = mapping->getOffset();
    }
    else
    {
        self.scaleFactor = 0.0;
        self.offset = 0.0;
    }
}

@end
