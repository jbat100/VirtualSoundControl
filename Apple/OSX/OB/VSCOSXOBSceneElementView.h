//
//  VSCOSXOBSceneElementView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 12/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCOB.h"
#include "VSCOSXEnvironmentController.h"

@interface VSCOSXOBSceneElementView : NSView

@property (nonatomic, assign) VSC::OB::Element_WPtr element;
@property (assign) id<VSCOSXEnvironmentController> environmentController;

+(CGFloat) defaultViewHeight;

-(void) reload;

@end
