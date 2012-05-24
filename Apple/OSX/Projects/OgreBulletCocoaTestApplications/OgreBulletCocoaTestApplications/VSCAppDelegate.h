//
//  VSCAppDelegate.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import <vector>

class VSCOgreBulletListener;
class VSCOgreBulletApplication;
class VSCOgreApplicationCocoaSetup;

@class OgreView;

@interface VSCAppDelegate : NSObject <NSApplicationDelegate> {
    
    VSCOgreBulletApplication* ogreBulletApplication;
    VSCOgreApplicationCocoaSetup* ogreApplicationCocoaSetup;
    std::vector<VSCOgreBulletListener*> ogreBulletScenes;
    
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet OgreView *ogreView;

-(void) startRendering;
-(void) stopRendering;

@end
