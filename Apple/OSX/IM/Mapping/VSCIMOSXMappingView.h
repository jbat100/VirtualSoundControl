//
//  VSCIMOSXMappingView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIM.h"
#include "VSCIMTarget.h"

#include <vector>

@protocol VSCIMOSXEventEditor;

@protocol VSCIMOSXMappingView <NSObject>

@property (assign) id<VSCIMOSXEventEditor> controller;
@property (nonatomic, assign) VSC::IM::Mapping_WPtr mapping;
@property (nonatomic, assign) VSC::IM::Target target;

@end


@interface VSCIMOSXMappingView : NSView <VSCIMOSXMappingView>
{
    @private
    VSC::IM::MappingTypeSet allowedMappingTypes;
}

+(CGFloat) defaultHeight;
+(CGFloat) heightOfViewForMapping:(VSC::IM::Mapping_SPtr)collisionMapping;

@property (assign) id<VSCIMOSXEventEditor> controller;
@property (weak) IBOutlet NSButton* editButton;

-(const VSC::IM::MappingTypeSet&) allowedMappingTypes;

-(void) reloadInterface;

@end

