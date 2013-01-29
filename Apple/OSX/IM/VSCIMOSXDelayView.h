//
//  VSCIMOSXDelayView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/30/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "VSCIMDelay.h"

@protocol VSCIMOSXEventChainController;


@interface VSCIMOSXDelayView : NSView

+(CGFloat) defaultViewHeight;

@property (nonatomic, assign) VSC::IM::Delay::WPtr delay;

@property (assign) IBOutlet id<VSCIMOSXEventChainController> eventChainController;

-(void) reloadInterface;

@end
