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
#import "VSCAppDelegate.h"

#import "NSString+VSCAdditions.h"
#import "NSArray+VSCAdditions.h"

#include "VSCOBApplication.h"
#include "VSCOBScene.h"

#include <Ogre/Ogre.h>

#include <vector>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#import "VSCOBOSXSceneDisplayView.h" // include last!

@interface VSCOSXEnvironmentWindowController ()

- (void) updateSceneChoice;

@end

@implementation VSCOSXEnvironmentWindowController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        // Initialization code here.
        
        VSC::OB::KeyboardManager::SPtr keyboardManager = VSC::OB::KeyboardManager::SPtr(new VSC::OB::KeyboardManager());
        
        self.keyBindings = keyboardManager->generateDefaultBindings();
        
        self.sceneController = VSC::OB::SceneController::SPtr(new VSC::OB::SceneController());
        
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    [self updateSceneChoice];
}


-(BOOL)acceptsFirstResponder {
    return NO;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
    return NO;
}

#pragma mark - Custom Setters

- (void) setApplicationManager:(id<VSCOSXApplicationManagerProtocol>) manager
{
    _applicationManager = manager;
    [self updateSceneChoice];
    
}

- (void) updateSceneChoice {
    
    if (self.applicationManager == nil || !self.applicationManager.ogreBulletApplication) {
        [self.scenePopUpButton removeAllItems];
        return;
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


- (IBAction)showMidiWindow:(id)sender
{
    [(VSCAppDelegate*)[NSApplication sharedApplication].delegate showMIDIWindow:sender];
}

- (IBAction)test1:(id)sender {
    
    //VSCOBOSXSceneDisplayView* ogreBulletSceneView = [self.ogreBulletSceneViews firstObject];
    
    BOOST_ASSERT(self.ogreBulletSceneView);
    
    BOOL accepts = [self.ogreBulletSceneView acceptsFirstResponder];
    NSLog(@"VSCOBOSXSceneDisplayView %@ accept first responder", accepts ? @"DOES" : @"DOES NOT");
    
    if (accepts) {
        BOOL isFirstResponder = [self.window firstResponder] != self.ogreBulletSceneView;
        NSLog(@"VSCOBOSXSceneDisplayView %@ first responder", isFirstResponder ? @"IS" : @"IS NOT");
    }
}

- (IBAction)test2:(id)sender {

}

- (IBAction)test3:(id)sender {
    
}


                                 
                                 

@end
