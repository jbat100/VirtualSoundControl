//
//  VSCIMOSXMappingCellView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIM.h"


#include <vector>

@protocol VSCIMOSXEventEditor;


@interface VSCIMOSXMappingCellView : NSView <VSCIMOSXMappingCellView>
{
    @private
    VSC::IM::MappingTypeSet mAllowedMappingTypes;
}

+(CGFloat) defaultHeight;
+(CGFloat) heightOfViewForMapping:(VSC::IM::Mapping_SPtr)collisionMapping;

@property (assign) id<VSCIMOSXEventEditor> controller;
@property (weak) IBOutlet NSButton* editButton;

-(VSC::IM::MappingTypeSet&) allowedMappingTypes;

-(void) reloadInterface;

@end

