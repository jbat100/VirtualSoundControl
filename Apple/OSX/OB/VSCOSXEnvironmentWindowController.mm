//
//  VSCOBOSXSceneWindow.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXEnvironmentWindowController.h"
#import "VSCOSXApplicationManager.h"
#import "VSCOBOSXSceneDisplayView.h"


#import "NSString+VSCAdditions.h"
#import "NSArray+VSCAdditions.h"

#include "VSCEnvironment.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"

#include <Ogre/Ogre.h>

#include <vector>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

@interface VSCOSXEnvironmentWindowController ()

@end

@implementation VSCOSXEnvironmentWindowController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        // Initialization code here.
        

        
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    
}


-(BOOL)acceptsFirstResponder {
    return NO;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
    return NO;
}

#pragma mark - Custom Setters

- (void) setEnvironment:(VSC::Environment::SPtr)environment
{
    _environment = environment;
    [self reloadInterface];
}


-(void) reloadInterface
{
    if (self.environment)
    {
        self.sceneView.scene = self.environment->getOBScene();
    }
    else
    {
        self.sceneView.scene = VSC::OB::Scene::SPtr();
    }
}



#pragma mark - Stupid tests

/*

- (void)keyDown:(NSEvent *)theEvent {
    NSLog(@"%@ keyDown %@", self, theEvent);
}

- (void)keyUp:(NSEvent *)theEvent {
    NSLog(@"%@ keyUp %@", self, theEvent);
}
 */

- (IBAction)test1:(id)sender {
    
}

- (IBAction)test2:(id)sender {

}

- (IBAction)test3:(id)sender {
    
}

@end
