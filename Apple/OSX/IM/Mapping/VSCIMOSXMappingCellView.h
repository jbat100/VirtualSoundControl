//
//  VSCIMOSXMappingCellView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIM.h"

@protocol VSCIMOSXEventEditor;

@interface VSCIMOSXMappingCellView : NSView
{
    @private
    VSC::IM::MappingTypeSet mAllowedMappingTypes;
}

+(CGFloat) defaultViewHeight;

@property (assign) id<VSCIMOSXEventEditor> controller;

@property (nonatomic, assign) VSC::IM::Mapping_WPtr mapping;

@property (nonatomic, weak) IBOutlet NSTextField* targetTextField;
@property (nonatomic, weak) IBOutlet NSPopUpButton* mappingTypePopUpButton;
@property (nonatomic, weak) IBOutlet NSButton* editButton;

-(IBAction) editMapping:(id)sender;
-(IBAction) mappingTypeSelected:(id)sender;

-(VSC::IM::MappingTypeSet&) allowedMappingTypes;
-(void) setAllowedMappingTypes:(VSC::IM::MappingTypeSet)mappingTypeSet;


@end

