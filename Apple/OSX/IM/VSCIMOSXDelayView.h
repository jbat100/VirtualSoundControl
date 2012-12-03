//
//  VSCIMOSXDelayView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/30/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "VSCIMDelay.h"

@interface VSCIMOSXDelayView : NSView

+(CGFloat) defaultViewHeight;

@property (nonatomic, assign) VSC::IM::Delay::WPtr delay;

@property (weak) id<VSCIMOSXCollisionEventChainController> eventChainController;

@end
