//
//  VSCIMOSXMappingEditViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXMappingEditViewController.h"

#include "VSCIMMapping.h"
#include <boost/assert.hpp>

@interface VSCIMOSXMappingEditViewController ()

-(void) reloadInterface;

@end

@implementation VSCIMOSXMappingEditViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        // Initialization code here.
    }
    
    return self;
}

-(void) setMapping:(VSC::IM::Mapping::WPtr)collisionMapping
{
    if (collisionMapping.lock() != _collisionMapping.lock())
    {
        _collisionMapping = collisionMapping;
        [self reloadInterface];
    }
}

-(void) reloadInterface
{
    VSC::IM::Mapping::SPtr mapping = _collisionMapping.lock();
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
