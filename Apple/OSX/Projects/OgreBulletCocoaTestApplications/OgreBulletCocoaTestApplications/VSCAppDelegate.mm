//
//  VSCAppDelegate.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//


#import "VSCAppDelegate.h"
#import "VSCOSXApplicationManager.h"

#import "VSCOBScene.h"
#import "VSCOBApplication.h"

#import "VSCOBPrimitivesDemo.h"
#import "VSCOBTriMeshDemo.h"
#import "VSCOBConstraintsDemo.h"


@implementation VSCAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    self.applicationManager = [[VSCOSXApplicationManager alloc] init];
    
    [self setupOgreBulletApplication];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification 
{
    [self teardownOgreBulletApplication];
}


-(void) setupOgreBulletApplication {
    
    VSC::OB::Application::Scenes scenes;
    
    scenes.push_back(VSC::OB::Scene::SPtr(new VSC::OB::PrimitivesDemo()));
    scenes.push_back(VSC::OB::Scene::SPtr(new VSC::OB::TriMeshDemo()));
	scenes.push_back(VSC::OB::Scene::SPtr(new VSC::OB::ConstraintsDemo()));
    
    VSC::OB::Application::SPtr ogreBulletApplication = VSC::OB::Application::SPtr(new VSC::OB::Application(scenes));
    
    self.applicationManager.ogreBulletApplication = ogreBulletApplication;
    
    [self.applicationManager startOgreRendering];
    
}

-(void) teardownOgreBulletApplication {
    
    /*
     *  Cleanup, setting the smart pointer to nil should take care of everything...
     */
    
    self.applicationManager.ogreBulletApplication = VSC::OB::Application::SPtr();
    
}


@end
