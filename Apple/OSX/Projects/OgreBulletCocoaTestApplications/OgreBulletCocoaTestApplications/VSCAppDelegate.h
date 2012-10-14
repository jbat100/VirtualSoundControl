//
//  VSCAppDelegate.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VSCOSXApplicationManager;

@interface VSCAppDelegate : NSObject <NSApplicationDelegate> 

@property (nonatomic, strong) VSCOSXApplicationManager* applicationManager;

-(IBAction)showMIDIWindow:(id)sender;
-(IBAction)showSceneWindow:(id)sender;

@end
