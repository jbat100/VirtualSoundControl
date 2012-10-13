//
//  VSCAppDelegate.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCApplicationManager.h"

#import <Cocoa/Cocoa.h>


@interface VSCAppDelegate : NSObject <NSApplicationDelegate> 

@property (nonatomic, strong) VSCApplicationManager* applicationManager;

-(void) setupOgreBulletApplication;
-(void) teardownOgreBulletApplication;

@end
