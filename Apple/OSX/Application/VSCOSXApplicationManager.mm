//
//  VSCOSXApplicationManager.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/13/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXApplicationManager.h"
#import "VSCOSXMIDIWindowController.h"
#import "VSCOBOSXSceneWindowController.h"
#import "VSCOBOSXSceneView.h"
#import "NSArray+VSCAdditions.h"

#include <boost/assert.hpp>

@interface VSCOSXApplicationManager ()

@property (nonatomic, strong) NSTimer* renderTimer;

@end

@implementation VSCOSXApplicationManager

@synthesize midiOutputManager = _midiOutputManager;
@synthesize ogreBulletApplication = _ogreBulletApplication;

-(id) init {
    
    if ((self = [super init])) {
        
        self.ogreBulletSceneWindowController = [[VSCOBOSXSceneWindowController alloc] initWithWindowNibName:@"VSCOBOSXSceneWindow"];
        BOOST_ASSERT(self.ogreBulletSceneWindowController);
        self.ogreBulletSceneWindowController.applicationManager = self;
        
        NSWindow* w = self.ogreBulletSceneWindowController.window;
        NSLog(@"OgreBulletSceneWindow: %@", w);
        
        self.midiWindowController = [[VSCOSXMIDIWindowController alloc] initWithWindowNibName:@"VSCOSXMIDIWindow"];
        BOOST_ASSERT(self.midiWindowController);
        self.midiWindowController.applicationManager = self;
        
    }
    
    return self;    
}

-(void) setOgreBulletApplication:(VSC::OB::Application::SPtr)application {
    
    _ogreBulletApplication = application;
    
    
    if (_ogreBulletApplication)
    {
        /*
         *  A bit hacky for now, plan on making this better
         */
        //VSCOBOSXSceneView* sceneView = [self.ogreBulletSceneWindowController.ogreBulletSceneViews firstObject];
        VSCOBOSXSceneView* sceneView = self.ogreBulletSceneWindowController.ogreBulletSceneView;
        
        BOOST_ASSERT(sceneView);
        
        _ogreBulletApplication->setupWithOgreView((__bridge void*)sceneView);
        
        /*
         *  Set the windows scene to the application's scene
         */
        self.ogreBulletSceneWindowController.ogreBulletScene = _ogreBulletApplication->getCurrentScene();
    }
    
}

-(void) startOgreRendering {
    
    if (self.renderTimer == nil) {
        
        // create a timer that causes OGRE to render at 50fps
        self.renderTimer = [NSTimer scheduledTimerWithTimeInterval:0.02 target:self
                                                          selector:@selector(renderCallback)
                                                          userInfo:NULL
                                                           repeats:YES];
        
    }
    
}

-(void) stopOgreRendering {
    
    [self.renderTimer invalidate];
    self.renderTimer = nil;
    
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
