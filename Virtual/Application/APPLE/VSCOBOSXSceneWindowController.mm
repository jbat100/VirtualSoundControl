//
//  VSCOBOSXSceneWindow.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOBOSXSceneWindowController.h"
#import "VSCOSXApplicationManager.h"
#import "VSCOBOSXSceneView.h"

#import "NSString+VSCAdditions.h"
#import "NSArray+VSCAdditions.h"

#include "VSCOBApplication.h"
#include "VSCOBScene.h"

#include <Ogre/Ogre.h>

#include <vector>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#import "VSCOBOSXSceneView.h" // include last!

@interface VSCOBOSXSceneWindowController ()

- (void) updateSceneChoice;

@end

@implementation VSCOBOSXSceneWindowController

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
    [self updateSceneChoice];
}

/*
-(BOOL)acceptsFirstResponder {
    return YES;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
    return YES;
}
 */

#pragma mark - Custom Setters

- (void) setApplicationManager:(id<VSCOSXApplicationManagerProtocol>) manager
{
    _applicationManager = manager;
    [self updateSceneChoice];
    
}

- (void) updateSceneChoice {
    
    if (self.applicationManager == nil || !self.applicationManager.ogreBulletApplication) {
        [self.scenePopUpButton removeAllItems];
    }
    
    std::vector<Ogre::String> sceneNames = self.applicationManager.ogreBulletApplication->getSceneNames();
    
    NSMutableArray* sceneStrings = [NSMutableArray array];
    BOOST_FOREACH (Ogre::String sceneName, sceneNames)
    {
        [sceneStrings addObject:[NSString stringWithStdString:sceneName]];
    }
    
    [self.scenePopUpButton removeAllItems];
    [self.scenePopUpButton addItemsWithTitles:sceneStrings];
    
    /*
     *  Set selected popup item to the current scene
     */
    
    if (self.applicationManager.ogreBulletApplication->getCurrentScene())
    {
        Ogre::String selectedSceneName = self.applicationManager.ogreBulletApplication->getCurrentScene()->getName();
        [self.scenePopUpButton selectItemWithTitle:[NSString stringWithStdString:selectedSceneName]];
    }
    
}

#pragma mark - UI Events

- (IBAction)scenePopUpButtonChanged:(id)sender 
{
    NSString* sceneName = [self.scenePopUpButton titleOfSelectedItem];
    bool success = self.applicationManager.ogreBulletApplication->switchToSceneWithName([sceneName stdString]);
    BOOST_ASSERT_MSG(success == true, "Expected  success");
}

#pragma mark - Stupid tests

- (void)keyDown:(NSEvent *)theEvent {
    NSLog(@"%@ keyDown %@", self, theEvent);
}

- (void)keyUp:(NSEvent *)theEvent {
    NSLog(@"%@ keyUp %@", self, theEvent);
}


- (IBAction)test1:(id)sender {
    
    VSCOBOSXSceneView* ogreBulletSceneView = [self.ogreBulletSceneViews firstObject];
    
    BOOST_ASSERT(ogreBulletSceneView);
    
    BOOL accepts = [ogreBulletSceneView acceptsFirstResponder];
    NSLog(@"VSCOBOSXSceneView %@ accept first responder", accepts ? @"DOES" : @"DOES NOT");
    
    if (accepts) {
        BOOL isFirstResponder = [self.window firstResponder] != ogreBulletSceneView;
        NSLog(@"VSCOBOSXSceneView %@ first responder", isFirstResponder ? @"IS" : @"IS NOT");
    }
}

- (IBAction)test2:(id)sender {

}

- (IBAction)test3:(id)sender {
    
}


                                 
                                 

@end
