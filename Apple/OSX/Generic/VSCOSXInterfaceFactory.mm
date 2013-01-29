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

@interface VSCOSXInterfaceFactory ()

-(NSView*) newViewWithIdentifier:(NSString*)identifier nib:(NSNib*)nib owner:(id)owner;

@end

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
        
        interfaceFactory.keyed3FieldViewNib = [[NSNib alloc] initWithNibNamed:
                                               [[VSCOSXKeyed3FieldView class] description] bundle:nil];
        
        interfaceFactory.keyed4FieldViewNib = [[NSNib alloc] initWithNibNamed:
                                               [[VSCOSXKeyed4FieldView class] description] bundle:nil];
        
        interfaceFactory.keyedActionCommonViewNib = [[NSNib alloc] initWithNibNamed:
                                                              @"VSCIMOSXActionCommonView" bundle:nil];
        
        interfaceFactory.keyedActionMIDIViewNib = [[NSNib alloc] initWithNibNamed:
                                                            @"VSCIMOSXActionMIDIView" bundle:nil];
        
        interfaceFactory.keyedActionMIDIControlViewNib = [[NSNib alloc] initWithNibNamed:
                                                                   @"VSCIMOSXActionMIDIControlView" bundle:nil];
        
    });
    
    BOOST_ASSERT(interfaceFactory);
    
    return interfaceFactory;
}

#pragma mark Generic Views

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

-(VSCOSXKeyed3FieldView*) newKeyed3FieldViewWithOwner:(id)owner
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

-(VSCOSXKeyed4FieldView*) newKeyed4FieldViewWithOwner:(id)owner
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

#pragma mark Collision Action Views

-(NSView*) newViewWithIdentifier:(NSString*)identifier nib:(NSNib*)nib owner:(id)owner
{    
    BOOST_ASSERT(nib);
    NSArray *objects = nil;
    NSView* v = nil;
    [nib instantiateNibWithOwner:owner topLevelObjects:&objects];
    for(id object in objects)
    {
        if([object isKindOfClass:[NSView class]])
        {
            NSView* local = (NSView*)object;
            if ([local.identifier isEqualToString:identifier])
            {
                v = local;
            }
            break;
        }
    }
    BOOST_ASSERT(v);
    v.translatesAutoresizingMaskIntoConstraints = NO;
    
    for (NSView* subview in v.subviews)
    {
        subview.translatesAutoresizingMaskIntoConstraints = NO;
    }
    
    return v;
}

-(NSView*) newActionCommonViewWithOwner:(id)owner
{
    static NSString* identifier = @"VSCIMOSXActionCommonView";
    
    return [self newViewWithIdentifier:identifier nib:self.keyedActionCommonViewNib owner:owner];
}

-(NSView*) newActionMIDIViewWithOwner:(id)owner
{
    static NSString* identifier = @"VSCIMOSXActionMIDIView";
    
    return [self newViewWithIdentifier:identifier nib:self.keyedActionMIDIViewNib owner:owner];
}

-(NSView*) newActionMIDIControlViewWithOwner:(id)owner
{
    static NSString* identifier = @"VSCIMOSXActionMIDIControlView";
    
    return [self newViewWithIdentifier:identifier nib:self.keyedActionMIDIControlViewNib owner:owner];
}


@end
