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

using namespace VSC;
using namespace VSC::IM;


@interface VSCIMOSXMappingCellView ()

/*
 *  Interface elements common to all mappings
 */

@property (weak) IBOutlet NSTextField* targetTextField;
@property (weak) IBOutlet NSPopUpButton* mappingPopUpButton;

-(IBAction) editMapping:(id)sender;
-(IBAction) mappingTypeSelected:(id)sender;

-(void) updateInterfaceForNewMapping;
-(void) updateInterfaceForNewTarget;

@end

@implementation VSCIMOSXMappingCellView

@synthesize controller = _controller;
@synthesize mapping = _mapping;

//@synthesize target = _target;
//@synthesize trigger = _trigger;

+(void) initialize
{

}

+(CGFloat) defaultHeight
{
    return 56.0;
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

-(void) setMapping:(Mapping::WPtr)mapping
{
    Mapping::SPtr newMapping = mapping.lock();
    
    BOOST_ASSERT(newMapping);
    
    if (newMapping)
    {
        MappingTypeSet::iterator it = std::find(mAllowedMappingTypes.begin(), mAllowedMappingTypes.end(), newMapping->getMappingType());
        BOOST_ASSERT_MSG(it != mAllowedMappingTypes.end(), "Mapping type not allowed");
        mapping = Mapping::WPtr();
    }
    
    _mapping = mapping;
    
    [self updateInterfaceForNewMapping];
}

-(VSC::IM::MappingTypeSet&) allowedMappingTypes
{
    return mAllowedMappingTypes;
}

-(void) updateInterfaceForNewMapping
{
    Mapping::SPtr m = self.mapping.lock();
    BOOST_ASSERT(self.mappingPopUpButton);
    BOOST_ASSERT(m);
    if (m)
    {
        [self.mappingPopUpButton selectItemWithTitle:[NSString stringWithStdString:StringForMappingType(m->getMappingType())]];
    }
    else
    {
        [self.mappingPopUpButton selectItemWithTitle:@"__EMPTY__"];
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
    NSString* menuItemTitle = [[self.mappingPopUpButton selectedItem] title];
    MappingType requestedType = MappingTypeForString([menuItemTitle stdString]);
    BOOST_ASSERT(requestedType != VSC::IM::MappingTypeNone);
    Mapping::SPtr m = self.mapping.lock();
    BOOST_ASSERT(m);
    if (m)
    {
        m->setMappingType(requestedType);
    }
}


@end
