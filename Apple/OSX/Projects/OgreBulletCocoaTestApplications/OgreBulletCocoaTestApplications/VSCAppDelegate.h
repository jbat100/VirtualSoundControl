//
//  VSCAppDelegate.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "OgrePrerequisites.h"
#import "OgreOSXCocoaView.h"

#import <Cocoa/Cocoa.h>

#import <vector>

class VSC::OB::Scene;
class VSC::OB::Application;

@class VSCOBOSXWindow;
@class VSCOBOSXView;

@interface VSCAppDelegate : NSObject <NSApplicationDelegate> {
    
    VSC::OB::Application* ogreBulletApplication;
    std::vector<VSC::OB::Scene*> ogreBulletScenes;
    
}

@property (assign) IBOutlet VSCOBOSXWindow *window;
@property (assign) IBOutlet VSCOBOSXView *ogreView;

-(void) startRendering;
-(void) stopRendering;

@end
