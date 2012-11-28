//
//  VSCIMOSXCollisionMappingView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionMappingView.h"
#import "VSCIMOSXCollisionMappingTypes.h"
#import "VSCIMOSXCollisionActionMappingsController.h"

#include "VSCIMTarget.h"
#include "VSCIMCollisionMapping.h"

NSDictionary* mappingTypeMenuItemStringDict = nil;


@interface VSCIMOSXCollisionMappingView ()

/*
 *  Interface elements common to all mappings
 */

@property (weak) IBOutlet NSTextField* targetTextField;
@property (weak) IBOutlet NSPopUpButton* mappingPopUpButton;
@property (weak) IBOutlet NSButton* editButton;

@property (assign) VSCIMOSXCollisionMappingType collisionMappingType;

-(IBAction) editMapping:(id)sender;
-(IBAction) mappingTypeSelected:(id)sender;

-(void) setupCollisionMappingChoice;
-(void) updateInterfaceForNewCollisionMapping;
-(void) updateInterfaceForNewTarget;

@end

@implementation VSCIMOSXCollisionMappingView

@synthesize controller = _controller;
@synthesize mapping = _mapping;
@synthesize target = _target;

+(void) initialize
{
    BOOST_ASSERT(!mappingTypeMenuItemStringDict);
    
    mappingTypeMenuItemStringDict  = @{
    @((int)VSCIMOSXCollisionMappingTypeNone)        : @"No Mapping",
    @((int)VSCIMOSXCollisionMappingTypeConstant)    : @"Constant Mapping",
    @((int)VSCIMOSXCollisionMappingTypeVelocity)    : @"Velocity Mapping",
    @((int)VSCIMOSXCollisionMappingTypeDistance)    : @"Distance Mapping"
    };
}

+(CGFloat) defaultHeightOfView
{
    return 40.0;
}

+(CGFloat) heightOfViewForCollisionMapping:(VSC::IM::CollisionMapping::SPtr)collisionMapping
{
    return 40.0;
}

+(NSString*) menuItemStringForCollisionMappingType:(VSCIMOSXCollisionMappingType)mappingType
{
    BOOST_ASSERT(mappingTypeMenuItemStringDict);
    return [mappingTypeMenuItemStringDict objectForKey:@((int)mappingType)];
}

+(VSCIMOSXCollisionMappingType) collisionMappingTypeForMenuItemString:(NSString*)menuItemString
{
    BOOST_ASSERT(mappingTypeMenuItemStringDict);
    
    NSSet* keys = [mappingTypeMenuItemStringDict keysOfEntriesPassingTest:^BOOL(id key, id obj, BOOL *stop) {
        if ([obj isKindOfClass:[NSString class]] == NO) return false;
        if ([(NSString*)obj isEqualToString:menuItemString]) return true;
        return false;
    }];
    
    BOOST_ASSERT([keys count] < 2);
    
    if ([keys count] == 1) return (VSCIMOSXCollisionMappingType)[[keys anyObject] intValue];
    
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

-(void) awakeFromNib
{
    [self setupCollisionMappingChoice];
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void) setMapping:(VSC::IM::CollisionMapping::WPtr)mapping
{
    _mapping = mapping;
    self.collisionMappingType = VSCIMOSXCollisionMappingTypeForCollisionMapping(_mapping.lock());
    
    [self updateInterfaceForNewCollisionMapping];
}

-(void) setTarget:(VSC::IM::Target)target
{
    _target = target;
    [self updateInterfaceForNewTarget];
}

-(void) updateInterfaceForNewTarget
{
    
}

-(void) updateInterfaceForNewCollisionMapping
{
    NSString* title = [[self class] menuItemStringForCollisionMappingType:self.collisionMappingType];
    
    [self.mappingPopUpButton selectItemWithTitle:title];
    
    switch (self.collisionMappingType) {
            
        case VSCIMOSXCollisionMappingTypeConstant:
            break;
            
        case VSCIMOSXCollisionMappingTypeVelocity:
            break;
            
        case VSCIMOSXCollisionMappingTypeDistance:
            break;
            
        default:
            break;
    }
    
}

-(void) setupCollisionMappingChoice
{
    
}

#pragma mark - UI Callbacks

-(IBAction) editMapping:(id)sender
{
    
}

-(IBAction) mappingTypeSelected:(id)sender
{
    BOOST_ASSERT(self.target != VSC::IM::TargetNone);
    BOOST_ASSERT(self.controller);
    BOOST_ASSERT([self.controller respondsToSelector:@selector(sender:requestsMappingWithType:forTarget:)]);
    
    VSC::IM::CollisionMapping::SPtr newMapping = VSC::IM::CollisionMapping::SPtr();
    
    if (self.target != VSC::IM::TargetNone)
    {
        NSString* menuItemTitle = [[self.mappingPopUpButton selectedItem] title];
        VSCIMOSXCollisionMappingType requestedType = [[self class] collisionMappingTypeForMenuItemString:menuItemTitle];
        BOOST_ASSERT(requestedType != VSCIMOSXCollisionMappingTypeNone);
        if (requestedType != VSCIMOSXCollisionMappingTypeNone)
        {
            newMapping = [self.controller sender:self requestsMappingWithType:requestedType forTarget:self.target];
        }
    }
    
    self.mapping = VSC::IM::CollisionMapping::WPtr(newMapping);
}


@end
