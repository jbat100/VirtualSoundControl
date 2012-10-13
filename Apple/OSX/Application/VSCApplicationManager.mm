//
//  VSCApplicationManager.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/13/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCApplicationManager.h"

@interface VSCApplicationManager ()

@property (nonatomic, strong) NSTimer* renderTimer;

@end

@implementation VSCApplicationManager

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
