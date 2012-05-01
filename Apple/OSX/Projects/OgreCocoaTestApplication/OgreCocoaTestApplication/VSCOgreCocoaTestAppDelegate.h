//
//  VSCAppDelegate.h
//  OgreCocoaTestApplication
//
//  Created by Jonathan Thorpe on 4/30/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

/**
 *  Setting up the application as described in 
 *  http://www.ogre3d.org/tikiwiki/Using+A+Cocoa+Window&structure=Cookbook
 *
 */

#import <Cocoa/Cocoa.h>
#import <Ogre/Ogre.h>
#import "OgreOSXCocoaView.h"

@interface VSCOgreCocoaTestAppDelegate : NSObject <NSApplicationDelegate> {

    Ogre::SceneNode *objectNode;

}

@property (strong) IBOutlet NSWindow *window;

@property (strong) IBOutlet OgreView *ogreView;

@end
