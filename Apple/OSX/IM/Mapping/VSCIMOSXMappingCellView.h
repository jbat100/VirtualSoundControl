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

//@property (nonatomic, assign) VSC::IM::Target target;
//@property (nonatomic, assign) VSC::IM::Trigger trigger;

@property (nonatomic, weak) IBOutlet NSButton* editButton;

-(VSC::IM::MappingTypeSet&) allowedMappingTypes;

-(void) reloadInterface;
-(void) setupMappingChoice;

@end

