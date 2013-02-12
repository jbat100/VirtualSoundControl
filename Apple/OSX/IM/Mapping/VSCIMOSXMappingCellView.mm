//
//  VSCIMOSXMappingCellView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXMappingCellView.h"
#import "VSCIMOSXEventEditor.h"
#import "NSString+VSCAdditions.h"

#include "VSCIM.h"
#include "VSCIMMapping.h"

#include <boost/foreach.hpp>

//NSDictionary* mappingTypeMenuItemStringDict = nil;


@interface VSCIMOSXMappingCellView ()

/*
 *  Interface elements common to all mappings
 */

@property (weak) IBOutlet NSTextField* targetTextField;
@property (weak) IBOutlet NSPopUpButton* mappingPopUpButton;

@property (assign) VSC::IM::MappingType collisionMappingType;

-(IBAction) editMapping:(id)sender;
-(IBAction) mappingTypeSelected:(id)sender;

-(void) setupMappingChoice;
-(void) updateInterfaceForNewMapping;
-(void) updateInterfaceForNewTarget;

@end

@implementation VSCIMOSXMappingCellView

@synthesize controller = _controller;
@synthesize mapping = _mapping;
@synthesize target = _target;

+(void) initialize
{

}

+(CGFloat) defaultHeight
{
    return 56.0;
}

+(CGFloat) heightOfViewForMapping:(VSC::IM::Mapping::SPtr)collisionMapping
{
    return 56.0;
}

+(NSString*) menuItemStringForMappingType:(VSC::IM::MappingType)mappingType
{
    return [NSString stringWithStdString:VSC::IM::StringForMappingType(mappingType)];
}

+(VSC::IM::MappingType) collisionMappingTypeForMenuItemString:(NSString*)menuItemString
{
    return VSC::IM::MappingTypeForString([menuItemString stdString]);
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

#pragma mark - Setters and Getters

-(void) setMapping:(VSC::IM::Mapping::WPtr)mapping
{
    _mapping = mapping;
    [self updateInterfaceForNewMapping];
}

-(void) setTarget:(VSC::IM::Target)target
{
    _target = target;
    [self updateInterfaceForNewTarget];
}

-(VSC::IM::MappingTypeSet&) allowedMappingTypes
{
    return mAllowedMappingTypes;
}

-(void) updateInterfaceForNewTarget
{
    BOOST_ASSERT(self.targetTextField);
    [self.targetTextField setStringValue:[NSString stringWithStdString:VSC::IM::StringForTarget(self.target)]];
}

-(void) updateInterfaceForNewMapping
{
    BOOST_ASSERT(self.mappingPopUpButton);
    
    NSString* title = [[self class] menuItemStringForMappingType:self.collisionMappingType];
    
    [self.mappingPopUpButton selectItemWithTitle:title];
    
    switch (self.collisionMappingType)
    {
        case VSC::IM::MappingTypeConstant:
            break;
        case VSC::IM::MappingTypeCollisionVelocity:
            break;
        case VSC::IM::MappingTypeCollisionDistance:
            break;
        default:
            break;
    }
}

-(void) setupMappingChoice
{
    BOOST_ASSERT(self.mappingPopUpButton);
    [self.mappingPopUpButton removeAllItems];
    BOOST_FOREACH (const VSC::IM::MappingType& mappingType, mAllowedMappingTypes)
    {
        NSString* title = [NSString stringWithStdString:VSC::IM::StringForMappingType(mappingType)];
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
        VSC::IM::MappingType requestedType = [[self class] collisionMappingTypeForMenuItemString:menuItemTitle];
        BOOST_ASSERT(requestedType != VSC::IM::MappingTypeNone);
        if (requestedType != VSC::IM::MappingTypeNone)
        {
            newMapping = [self.controller sender:self requestsMappingWithType:requestedType forTarget:self.target];
        }
    }
    
    self.mapping = VSC::IM::Mapping::WPtr(newMapping);
}


@end
