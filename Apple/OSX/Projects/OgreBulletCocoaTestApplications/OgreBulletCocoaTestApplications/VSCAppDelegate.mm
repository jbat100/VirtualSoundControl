//
//  VSCAppDelegate.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//


#import "VSCAppDelegate.h"

#import <vector>

#import "VSC::OB::Scene.h"
#import "VSCOgreBulletApplication.h"
#import "VSCOBOSXWindow.h"
#import "VSCOgreApplicationCocoaSetup.h"

#import "VSCOgreBulletPrimitivesDemo.h"
#import "VSCOgreBulletTriMeshDemo.h"
#import "VSCOgreBulletVehiclesDemo.h"
#import "VSCOgreBulletConstraintsDemo.h"
#import "VSCOgreBulletTerrainDemo.h"
#import "VSCOgreBulletRagdollDemo.h"


using namespace OIS;

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
    
    ogreBulletScenes.push_back(new VSC::OB::PrimitivesDemo());
    ogreBulletScenes.push_back(new VSC::OB::TriMeshDemo());
	ogreBulletScenes.push_back(new VSC::OB::VehiclesDemo());
	ogreBulletScenes.push_back(new VSC::OB::ConstraintsDemo());
	ogreBulletScenes.push_back(new VSC::OB::TerrainDemo());
	ogreBulletScenes.push_back(new VSC::OB::RagdollDemo());
    
    // Create the application and try to run it
    ogreBulletApplication = new VSC::OB::Application(ogreBulletScenes);
    
    ogreBulletApplication->setupWithOgreView((__bridge void*)self.ogreView);
    
    self.window.ogreBulletApplication = ogreBulletApplication;
    
    [self startRendering];
    
}

-(void) teardownOgreBulletApplication {
    
    /*
     *  Cleanup
     */
    
    std::vector<VSC::OB::Scene*>::iterator it = ogreBulletScenes.begin();
    while (it != ogreBulletScenes.end())
    {
        delete *it;
        ++it;
    }
    
    delete ogreBulletApplication;    
    ogreBulletApplication = 0;
    
    
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
