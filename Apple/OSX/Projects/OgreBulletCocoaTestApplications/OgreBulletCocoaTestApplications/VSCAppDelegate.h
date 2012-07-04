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
class VSCOgreBulletCocoaInputAdapter;

@class VSCOgreView;

@interface VSCAppDelegate : NSObject <NSApplicationDelegate> {
    
    VSCOgreBulletApplication* ogreBulletApplication;
    VSCOgreApplicationCocoaSetup* ogreApplicationCocoaSetup;
    VSCOgreBulletCocoaInputAdapter* cocoaInputAdapter;
    std::vector<VSCOgreBulletListener*> ogreBulletScenes;
    
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet VSCOgreView *ogreView;

-(void) startRendering;
-(void) stopRendering;

@end
