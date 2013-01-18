//
//  VSCOSXInterfaceFactory.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/18/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCOSXInterfaceFactory.h"

#import "VSCOSXKeyedCheckBoxView.h"
#import "VSCOSXKeyedSliderView.h"

#include <boost/assert.hpp>

@implementation VSCOSXInterfaceFactory

+(VSCOSXInterfaceFactory*) defaultFactory
{
    static VSCOSXInterfaceFactory* interfaceFactory = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        BOOST_ASSERT(!interfaceFactory);
        
        interfaceFactory = [[VSCOSXInterfaceFactory alloc] init];
        
        interfaceFactory.keyedCheckBoxViewNib = [[NSNib alloc] initWithNibNamed:
                                                 [[VSCOSXKeyedCheckBoxView class] description] bundle:nil];
        
        interfaceFactory.keyedSliderViewNib = [[NSNib alloc] initWithNibNamed:
                                                 [[VSCOSXKeyedSliderView class] description] bundle:nil];
        
    });
    
    BOOST_ASSERT(interfaceFactory);
    
    return interfaceFactory;
}

-(VSCOSXKeyedCheckBoxView*) newKeyedCheckBoxViewWithOwner:(id)owner
{
    NSString* identifier = [[VSCOSXKeyedCheckBoxView class] description];
    BOOST_ASSERT(self.keyedCheckBoxViewNib);
    NSArray *objects = nil;
    VSCOSXKeyedCheckBoxView* v = nil;
    [self.keyedCheckBoxViewNib instantiateNibWithOwner:owner topLevelObjects:&objects];
    for(id object in objects)
    {
        if([object isKindOfClass:[VSCOSXKeyedCheckBoxView class]])
        {
            v = object;
            v.identifier = identifier;
            break;
        }
    }
    BOOST_ASSERT(v);
    v.translatesAutoresizingMaskIntoConstraints = NO;
    return v;
}

-(VSCOSXKeyedSliderView*) newKeyedSliderViewWithOwner:(id)owner
{
    NSString* identifier = [[VSCOSXKeyedSliderView class] description];
    BOOST_ASSERT(self.keyedSliderViewNib);
    NSArray *objects = nil;
    VSCOSXKeyedSliderView* v = nil;
    [self.keyedCheckBoxViewNib instantiateNibWithOwner:owner topLevelObjects:&objects];
    for(id object in objects)
    {
        if([object isKindOfClass:[VSCOSXKeyedSliderView class]])
        {
            v = object;
            v.identifier = identifier;
            break;
        }
    }
    BOOST_ASSERT(v);
    v.translatesAutoresizingMaskIntoConstraints = NO;
    return v;
}

@end
