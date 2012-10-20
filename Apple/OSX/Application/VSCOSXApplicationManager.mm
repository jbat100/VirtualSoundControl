//
//  VSCOSXApplicationManager.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/13/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXApplicationManager.h"
#import "VSCOSXMIDIWindowController.h"
#import "VSCOSXEnvironmentWindowController.h"
#import "VSCOBOSXSceneDisplayView.h"
#import "NSArray+VSCAdditions.h"

#include <Ogre/Ogre.h>

#include <boost/assert.hpp>

@interface VSCOSXApplicationManager ()

@property (nonatomic, strong) NSTimer* renderTimer;

@end

@implementation VSCOSXApplicationManager

@synthesize application = _application;

-(id) init {
    
    if ((self = [super init])) {
        
        self.application = VSC::GlobalApplication::SPtr(new VSC::GlobalApplication);
        
        self.application->init();
    }
    
    return self;    
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
    //Ogre::Root::getSingleton().renderOneFrame();
    
    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    
    BOOST_ASSERT(root);
    
    if (root)
    {
        root->renderOneFrame();
    }
    
}

@end
