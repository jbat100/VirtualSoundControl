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
#import "VSCOBOSXWindow.h"
#import "VSCOBOSXApplicationSetup.h"

#import "VSCOBPrimitivesDemo.h"
#import "VSCOBTriMeshDemo.h"
#import "VSCOBConstraintsDemo.h"

@interface VSCAppDelegate () {
    
    NSTimer* renderTimer;
    
}

-(void) setupOgreBulletApplication;
-(void) teardownOgreBulletApplication;

@end


@implementation VSCAppDelegate

@synthesize window = _window;
@synthesize ogreView = _ogreView;

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

-(void) startRendering {
    
    if (renderTimer == nil) {
        
        // create a timer that causes OGRE to render at 50fps
        renderTimer = [NSTimer scheduledTimerWithTimeInterval:0.02 target:self 
                                                     selector:@selector(renderCallback) 
                                                     userInfo:NULL 
                                                      repeats:YES];
        
    }
    
}

-(void) stopRendering {
    
    [renderTimer invalidate];
    renderTimer = nil;
    
}

-(void) renderCallback {
    
    /*
     *  The singleton accessed here is the one created in the ogreApplicationCocoaSetup call to setup
     *
     *  see http://www.ogre3d.org/docs/api/html/classOgre_1_1Root.html#_details
     */
    
    //std::cout << "In renderCallback ..." << std::endl;
    
    Ogre::Root::getSingleton().renderOneFrame();
    
}

@end
