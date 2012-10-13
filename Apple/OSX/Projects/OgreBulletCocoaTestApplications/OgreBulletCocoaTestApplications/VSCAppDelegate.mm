//
//  VSCAppDelegate.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//


#import "VSCAppDelegate.h"

#import <vector>

#import "VSCOBScene.h"
#import "VSCOBApplication.h"
#import "VSCOBOSXSceneWindow.h"
#import "VSCOBOSXApplicationSetup.h"

#import "VSCOBPrimitivesDemo.h"
#import "VSCOBTriMeshDemo.h"
#import "VSCOBConstraintsDemo.h"


@implementation VSCAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [self setupOgreBulletApplication];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification 
{
    [self teardownOgreBulletApplication];
}


-(void) setupOgreBulletApplication {
    
    NSAssert(self.ogreView != nil, @"Expected OgreView");
    
    VSC::OB::Application::Scenes scenes;
    
    scenes.push_back(VSC::OB::Scene::SPtr(new VSC::OB::PrimitivesDemo()));
    scenes.push_back(VSC::OB::Scene::SPtr(new VSC::OB::TriMeshDemo()));
	scenes.push_back(VSC::OB::Scene::SPtr(new VSC::OB::ConstraintsDemo()));
    
    // Create the application and try to run it
    ogreBulletApplication = VSC::OB::Application::SPtr(new VSC::OB::Application(scenes));
    
    ogreBulletApplication->setupWithOgreView((__bridge void*)self.ogreView);
    
    self.window.ogreBulletApplication = ogreBulletApplication;
    
    [self startRendering];
    
}

-(void) teardownOgreBulletApplication {
    
    /*
     *  Cleanup, setting the smart pointer to nil should take care of everything...
     */
    
    ogreBulletApplication = VSC::OB::Application::SPtr();
    
}


@end
