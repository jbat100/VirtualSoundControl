//
//  VSCAppDelegate.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCOSXApplicationManagerProtocol.h"

@class VSCOSXEnvironmentWindowController;
@class VSCOSXMIDIWindowController;

@interface VSCAppDelegate : NSObject <NSApplicationDelegate> 

@property (nonatomic, strong) id<VSCOSXApplicationManagerProtocol> applicationManager;

@property (nonatomic, strong) VSCOSXEnvironmentWindowController *environmentWindowController;
@property (nonatomic, strong) VSCOSXMIDIWindowController *midiWindowController;

-(IBAction)showMIDIWindow:(id)sender;
-(IBAction)showEnvironmentWindow:(id)sender;

@end
