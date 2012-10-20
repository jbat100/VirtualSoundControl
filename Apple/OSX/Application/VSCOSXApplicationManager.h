//
//  VSCOSXApplicationManager.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/13/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCOSXApplicationManagerProtocol.h"

#include "VSCGlobalApplication.h"


@interface VSCOSXApplicationManager : NSObject <VSCOSXApplicationManagerProtocol>

@property(nonatomic, assign) VSC::GlobalApplication::SPtr application;

-(void) startOgreRendering;
-(void) stopOgreRendering;

@end
