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
#import "VSCOSXKeyed3FieldView.h"
#import "VSCOSXKeyed4FieldView.h"

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
    [self.keyedSliderViewNib instantiateNibWithOwner:owner topLevelObjects:&objects];
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

-(VSCOSXKeyed3FieldView*) newVSCOSXKeyed3FieldViewWithOwner:(id)owner
{
    NSString* identifier = [[VSCOSXKeyed3FieldView class] description];
    BOOST_ASSERT(self.keyed3FieldViewNib);
    NSArray *objects = nil;
    VSCOSXKeyed3FieldView* v = nil;
    [self.keyed3FieldViewNib instantiateNibWithOwner:owner topLevelObjects:&objects];
    for(id object in objects)
    {
        if([object isKindOfClass:[VSCOSXKeyed3FieldView class]])
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

-(VSCOSXKeyed4FieldView*) newVSCOSXKeyed4FieldViewWithOwner:(id)owner
{
    NSString* identifier = [[VSCOSXKeyed4FieldView class] description];
    BOOST_ASSERT(self.keyed4FieldViewNib);
    NSArray *objects = nil;
    VSCOSXKeyed4FieldView* v = nil;
    [self.keyed4FieldViewNib instantiateNibWithOwner:owner topLevelObjects:&objects];
    for(id object in objects)
    {
        if([object isKindOfClass:[VSCOSXKeyed4FieldView class]])
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
