//
//  VSCIMOSXDelayCellView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/30/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXEventCellView.h"

#include "VSCIM.h"

@protocol VSCIMOSXEventChainController;


@interface VSCIMOSXDelayCellView : VSCIMOSXEventCellView

-(VSC::IM::Delay_SPtr) delay;

@property (weak) IBOutlet NSTextField* titleTextField;

@end
