//
//  VSCOBOSXWindow.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOBOSXWindow.h"
#import "NSString+VSCAdditions.h"

#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include <Ogre/Ogre.h>

#include <vector>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#import "VSCOBOSXView.h" // include last!

@implementation VSCOBOSXWindow

@synthesize ogreView = _ogreView;
@synthesize ogreBulletApplication = _ogreBulletApplication;
@synthesize scenePopUpButton = _scenePopUpButton;

-(BOOL)acceptsFirstResponder {
    return YES;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
    return YES;
}

#pragma mark - Custom Setters

- (void) setOgreBulletApplication:(VSC::OB::Application::WPtr)app {
    // update the popup button with the scenes
    
    _ogreBulletApplication = app;
    
    std::vector<Ogre::String> sceneNames = app->getSceneNames();
    
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
    
    if (app->getCurrentScene()) 
    {
        Ogre::String selectedSceneName = app->getCurrentScene()->getName();
        [self.scenePopUpButton selectItemWithTitle:[NSString stringWithStdString:selectedSceneName]];
    }
    
}

#pragma mark - UI Events

- (IBAction)scenePopUpButtonChanged:(id)sender 
{
    NSString* sceneName = [self.scenePopUpButton titleOfSelectedItem];
    bool success = self.ogreBulletApplication->switchToSceneWithName([sceneName stdString]);
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
    BOOL accepts = [self.ogreView acceptsFirstResponder];
    NSLog(@"VSCOBOSXView %@ accept first responder", accepts ? @"DOES" : @"DOES NOT");
    
    if (accepts) {
        BOOL isFirstResponder = [self firstResponder] != self.ogreView;
        NSLog(@"VSCOBOSXView %@ first responder", isFirstResponder ? @"IS" : @"IS NOT");
    }
}

- (IBAction)test2:(id)sender {
    BOOL isFirstResponder = [self firstResponder] != self.ogreView;
    if (isFirstResponder) {
        [self.ogreView resignFirstResponder];
    }
    else {
        [self.ogreView becomeFirstResponder];
    }
    isFirstResponder = [self firstResponder] != self.ogreView;
     NSLog(@"VSCOBOSXView %@ first responder", isFirstResponder ? @"IS" : @"IS NOT");
}

- (IBAction)test3:(id)sender {
    
}


                                 
                                 

@end
