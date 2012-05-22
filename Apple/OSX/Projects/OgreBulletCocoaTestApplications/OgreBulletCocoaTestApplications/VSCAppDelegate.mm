//
//  VSCAppDelegate.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCAppDelegate.h"
#import <vector>

#import "VSCOgreBulletApplication.h"
#import "VSCOgreApplicationCocoaSetup.h"

#import "VSCOgreBulletListener.h"
#import "VSCOgreBulletPrimitivesDemo.h"
#import "VSCOgreBulletTriMeshDemo.h"
#import "VSCOgreBulletVehiclesDemo.h"
#import "VSCOgreBulletConstraintsDemo.h"
#import "VSCOgreBulletTerrainDemo.h
#import "VSCOgreBulletRagdollDemo.h"

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
    
    ogreBulletScenes.push_back(new VSCOgreBulletPrimitivesDemo());
    ogreBulletScenes.push_back(new VSCOgreBulletTriMeshDemo());
	ogreBulletScenes.push_back(new VSCOgreBulletVehiclesDemo());
	ogreBulletScenes.push_back(new VSCOgreBulletConstraintsDemo());
	ogreBulletScenes.push_back(new VSCOgreBulletTerrainDemo());
	ogreBulletScenes.push_back(new VSCOgreBulletRagdollDemo());
    
    ogreBulletScenes[0]->setNextKey(KC_F1);
    ogreBulletScenes[1]->setNextKey(KC_F2);
	ogreBulletScenes[2]->setNextKey(KC_F3);
	ogreBulletScenes[3]->setNextKey(KC_F4);
	ogreBulletScenes[4]->setNextKey(KC_F5);
	ogreBulletScenes[5]->setNextKey(KC_F6);
    
    // Create the application and try to run it
    ogreBulletApplication = new VSCOgreBulletApplication(&ogreBulletScenes);
    ogreApplicationCocoaSetup = new VSCOgreApplicationCocoaSetup();
    ogreApplicationCocoaSetup->setOgreView(self.ogreView);
    ogreBulletApplication->setup(ogreBulletApplication);
    
}

-(void) teardownOgreBulletApplication {
    
    /*
     *  Cleanup
     */
    
    std::vector<VSCOgreBulletListener*>::iterator it =  ogreBulletScenes.begin();
    while (it != ogreBulletScenes.end())
    {
        delete *it;
        ++it;
    }
    
    delete ogreBulletApplication;
    delete ogreApplicationCocoaSetup;
    
    ogreBulletApplication = 0;
    
}

-(void) startRendering {
    
    if (renderTimer == nil) {
        
        // create a timer that causes OGRE to render at 50fps
        renderTimer = [NSTimer scheduledTimerWithTimeInterval:0.02 target:self 
                                                     selector:@selector(renderFrame) 
                                                     userInfo:NULL 
                                                      repeats:YES];
        
    }
    
}

-(void) stopRendering {
    
    [renderTime invalidate];
    renderTimer = nil;
    
}

-(void) renderCallback {
    
    /*
     *  The singleton accessed here is the one created in the ogreApplicationCocoaSetup call to setup
     *
     *  see http://www.ogre3d.org/docs/api/html/classOgre_1_1Root.html#_details
     */
    
    Ogre::Root::getSingleton().renderOneFrame();
    
}

@end
