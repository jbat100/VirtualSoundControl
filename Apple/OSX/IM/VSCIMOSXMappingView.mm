//
//  VSCIMOSXMappingView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXMappingView.h"
#import "VSCIMOSXMappingTypes.h"
#import "VSCIMOSXActionMappingsController.h"
#import "NSString+VSCAdditions.h"

#include "VSCIMTarget.h"
#include "VSCIMMapping.h"

NSDictionary* mappingTypeMenuItemStringDict = nil;


@interface VSCIMOSXMappingView ()

/*
 *  Interface elements common to all mappings
 */

@property (weak) IBOutlet NSTextField* targetTextField;
@property (weak) IBOutlet NSPopUpButton* mappingPopUpButton;

@property (assign) VSCIMOSXMappingType collisionMappingType;

-(IBAction) editMapping:(id)sender;
-(IBAction) mappingTypeSelected:(id)sender;

-(void) setupMappingChoice;
-(void) updateInterfaceForNewMapping;
-(void) updateInterfaceForNewTarget;

@end

@implementation VSCIMOSXMappingView

@synthesize controller = _controller;
@synthesize mapping = _mapping;
@synthesize target = _target;

+(void) initialize
{
    BOOST_ASSERT(!mappingTypeMenuItemStringDict);
    
    mappingTypeMenuItemStringDict = @{
    @((int)VSCIMOSXMappingTypeNone)        : @"No Mapping",
    @((int)VSCIMOSXMappingTypeConstant)    : @"Constant Mapping",
    @((int)VSCIMOSXMappingTypeVelocity)    : @"Velocity Mapping",
    @((int)VSCIMOSXMappingTypeDistance)    : @"Distance Mapping"
    };
}

+(CGFloat) defaultHeight
{
    return 56.0;
}

+(CGFloat) heightOfViewForMapping:(VSC::IM::Mapping::SPtr)collisionMapping
{
    return 56.0;
}

+(NSString*) menuItemStringForMappingType:(VSCIMOSXMappingType)mappingType
{
    BOOST_ASSERT(mappingTypeMenuItemStringDict);
    return [mappingTypeMenuItemStringDict objectForKey:@((int)mappingType)];
}

+(VSCIMOSXMappingType) collisionMappingTypeForMenuItemString:(NSString*)menuItemString
{
    BOOST_ASSERT(mappingTypeMenuItemStringDict);
    
    NSSet* keys = [mappingTypeMenuItemStringDict keysOfEntriesPassingTest:^BOOL(id key, id obj, BOOL *stop) {
        if ([obj isKindOfClass:[NSString class]] == NO) return false;
        if ([(NSString*)obj isEqualToString:menuItemString]) return true;
        return false;
    }];
    
    BOOST_ASSERT([keys count] < 2);
    
    if ([keys count] == 1) return (VSCIMOSXMappingType)[[keys anyObject] intValue];
    
    BOOST_ASSERT_MSG(false, "No Mapping Type");
    
    return VSCIMOSXMappingTypeNone;
}

#pragma mark - NSView Methods

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
    [self setupMappingChoice];
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void) setMapping:(VSC::IM::Mapping::WPtr)mapping
{
    _mapping = mapping;
    self.collisionMappingType = VSCIMOSXMappingTypeForMapping(_mapping.lock());
    
    [self updateInterfaceForNewMapping];
}

-(void) setTarget:(VSC::IM::Target)target
{
    _target = target;
    [self updateInterfaceForNewTarget];
}

-(void) updateInterfaceForNewTarget
{
    BOOST_ASSERT(self.targetTextField);
    [self.targetTextField setStringValue:[NSString stringWithStdString:VSC::IM::stringForTarget(self.target)]];
}

-(void) updateInterfaceForNewMapping
{
    BOOST_ASSERT(self.mappingPopUpButton);
    
    NSString* title = [[self class] menuItemStringForMappingType:self.collisionMappingType];
    
    [self.mappingPopUpButton selectItemWithTitle:title];
    
    switch (self.collisionMappingType)
    {
        case VSCIMOSXMappingTypeConstant:
            break;
        case VSCIMOSXMappingTypeVelocity:
            break;
        case VSCIMOSXMappingTypeDistance:
            break;
        default:
            break;
    }
}

-(void) setupMappingChoice
{
    BOOST_ASSERT(self.mappingPopUpButton);
    
    [self.mappingPopUpButton removeAllItems];
    
    for (NSNumber* typeNumber in [mappingTypeMenuItemStringDict allKeys])
    {
        NSString* title = [[self class] menuItemStringForMappingType:(VSCIMOSXMappingType)[typeNumber intValue]];
        [self.mappingPopUpButton addItemWithTitle:title];
    }
}

-(void) reloadInterface;
{
    [self updateInterfaceForNewMapping];
    [self updateInterfaceForNewTarget];
}

#pragma mark - UI Callbacks

-(IBAction) editMapping:(id)sender
{
    BOOST_ASSERT(self.controller);
    BOOST_ASSERT([self.controller respondsToSelector:@selector(sender:requestsEditorForMapping:)]);
    
    if ([self.controller respondsToSelector:@selector(sender:requestsEditorForMapping:)])
    {
        VSC::IM::Mapping::SPtr mapping = self.mapping.lock();
        BOOST_ASSERT(mapping);
        [self.controller sender:self requestsEditorForMapping:mapping];
    }
}

-(IBAction) mappingTypeSelected:(id)sender
{
    BOOST_ASSERT(self.target != VSC::IM::TargetNone);
    BOOST_ASSERT(self.controller);
    BOOST_ASSERT([self.controller respondsToSelector:@selector(sender:requestsMappingWithType:forTarget:)]);
    
    VSC::IM::Mapping::SPtr newMapping = VSC::IM::Mapping::SPtr();
    
    if (self.target != VSC::IM::TargetNone)
    {
        NSString* menuItemTitle = [[self.mappingPopUpButton selectedItem] title];
        VSCIMOSXMappingType requestedType = [[self class] collisionMappingTypeForMenuItemString:menuItemTitle];
        BOOST_ASSERT(requestedType != VSCIMOSXMappingTypeNone);
        if (requestedType != VSCIMOSXMappingTypeNone)
        {
            newMapping = [self.controller sender:self requestsMappingWithType:requestedType forTarget:self.target];
        }
    }
    
    self.mapping = VSC::IM::Mapping::WPtr(newMapping);
}


@end
