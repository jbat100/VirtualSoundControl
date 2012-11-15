//
//  VSCOBOSXSceneDisplayView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 6/22/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOBOSXSceneDisplayView.h"

#include "VSCOBOSXInterfaceAdapter.h"

#include "VSCOBApplication.h"
#include "VSCOBSceneController.h"
#include "VSCOBDisplayController.h"
#include "VSCOBKeyBindings.h"
#include "VSCOBKeyboardManager.h"

#include <boost/assert.hpp>

@interface VSCOBOSXSceneDisplayView ()

@property (nonatomic, assign) VSC::OB::Display::WPtr display;

@property (nonatomic, assign) VSC::OB::OSXInterfaceAdapter::SPtr inputAdapter;

@property (nonatomic, assign) VSC::OB::SceneController::SPtr sceneController;
@property (nonatomic, assign) VSC::OB::DisplayController::SPtr displayController;

@end

@implementation VSCOBOSXSceneDisplayView

static const bool mTraceUI = false;

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    
    if (self) {
        
        VSC::OB::Display::SPtr d = VSC::OB::Application::singletonApplication()->createDisplayWithView<VSC::OB::Display>((__bridge void*)self);
        self.display = VSC::OB::Display::WPtr(d);
        
        VSC::OB::KeyboardManager::SPtr keyboardManager = VSC::OB::KeyboardManager::SPtr(new VSC::OB::KeyboardManager());
        VSC::OB::KeyBindings::SPtr keyBindings = keyboardManager->generateDefaultBindings();
        
        self.interfaceResponderChain = VSC::OB::InterfaceResponderChain::SPtr(new VSC::OB::InterfaceResponderChain);
        
        self.sceneController = VSC::OB::SceneController::SPtr(new VSC::OB::SceneController);
        self.sceneController->setOgreKeyBindings(keyBindings);
        
        self.displayController = VSC::OB::DisplayController::SPtr(new VSC::OB::DisplayController);
        self.displayController->setOgreKeyBindings(keyBindings);
        
        self.displayController->setupWithDisplay(self.display.lock());
        
        self.interfaceResponderChain->appendResponder(boost::static_pointer_cast<VSC::OB::InterfaceResponder>(self.sceneController));
        self.interfaceResponderChain->appendResponder(boost::static_pointer_cast<VSC::OB::InterfaceResponder>(self.displayController));
        
        self.inputAdapter = VSC::OB::OSXInterfaceAdapter::SPtr(new VSC::OB::OSXInterfaceAdapter());
        self.inputAdapter->allowRapidFireKeyPresses(false);
        self.inputAdapter->addResponder(self.interfaceResponderChain);
        self.inputAdapter->setCocoaView(self);
        
        // Initialization code here.
        
        [self setPostsFrameChangedNotifications:YES];
        [self setPostsBoundsChangedNotifications:YES];
        
        [[NSNotificationCenter defaultCenter] addObserverForName:NSViewBoundsDidChangeNotification
                                                          object:nil
                                                           queue:nil
                                                      usingBlock:^(NSNotification *)
        {
            if (self.inputAdapter) self.inputAdapter->contextChanged([self ogreWindow], nil);
        }
         ];
        
        [[NSNotificationCenter defaultCenter] addObserverForName:NSViewFrameDidChangeNotification
                                                          object:nil
                                                           queue:nil
                                                      usingBlock:^(NSNotification *)
         {
             if (self.inputAdapter) self.inputAdapter->contextChanged([self ogreWindow], nil);
         }
         ];
        
    }
    
    return self;
}

-(void) dealloc {
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    
}

#pragma mark - Custom Setters

-(void) setScene:(VSC::OB::Scene::WPtr)scene
{
    VSC::OB::Scene::SPtr currentScene = _scene.lock();
    VSC::OB::Scene::SPtr newScene = scene.lock();
    
    if (currentScene != newScene) {
        
        _scene = scene;
        
        if (newScene)
        {
            self.display.lock()->setupWithScene(newScene);
            self.sceneController->setupWithScene(newScene);
            
        }
        else
        {
            self.display.lock()->shutdown();
            self.sceneController->shutdown();
        }
        
    }
    
}

#pragma mark - NSResponder Stuff

-(BOOL)acceptsFirstResponder {
    return YES;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
    return YES;
}

#pragma mark - Keyboard

- (void)keyDown:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ keyDown: %@", self, theEvent);
    BOOST_ASSERT_MSG(self.inputAdapter, "expected input adapter");
    self.inputAdapter->keyDown([self ogreWindow], theEvent);
}

- (void)keyUp:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ keyUp: %@", self, theEvent);
    self.inputAdapter->keyUp([self ogreWindow], theEvent); 
}

#pragma mark - Mouse

- (void)mouseEntered:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseEntered: %@", self, theEvent);
    self.inputAdapter->mouseEntered([self ogreWindow], theEvent); 
}

- (void)mouseExited:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseExited: %@", self, theEvent);
    self.inputAdapter->mouseExited([self ogreWindow], theEvent); 
}

- (void)mouseMoved:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseMoved: %@", self, theEvent);
    self.inputAdapter->mouseMoved([self ogreWindow], theEvent); 
}

#pragma mark Left Mouse Button

- (void)mouseDown:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseDown: %@", self, theEvent);
    self.inputAdapter->mouseDown([self ogreWindow], theEvent);
}

- (void)mouseUp:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseUp: %@", self, theEvent);
    self.inputAdapter->mouseUp([self ogreWindow], theEvent);
}

- (void)mouseDragged:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseDragged: %@", self, theEvent);
    self.inputAdapter->mouseDragged([self ogreWindow], theEvent);
}

#pragma mark Right Mouse Button

- (void)rightMouseDown:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ rightMouseDown: %@", self, theEvent);
    self.inputAdapter->rightMouseDown([self ogreWindow], theEvent);
}

- (void)rightMouseUp:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ rightMouseUp: %@", self, theEvent);
    self.inputAdapter->rightMouseUp([self ogreWindow], theEvent);
}

- (void)rightMouseDragged:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ rightMouseDragged: %@", self, theEvent);
    self.inputAdapter->rightMouseDragged([self ogreWindow], theEvent);
}

#pragma mark Other Mouse Button

- (void)otherMouseDown:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ otherMouseDown: %@", self, theEvent);
    self.inputAdapter->otherMouseDown([self ogreWindow], theEvent);
}

- (void)otherMouseUp:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ otherMouseUp: %@", self, theEvent);
    self.inputAdapter->otherMouseUp([self ogreWindow], theEvent);
}

- (void)otherMouseDragged:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ otherMouseDragged: %@", self, theEvent);
    self.inputAdapter->otherMouseDragged([self ogreWindow], theEvent);
}

#pragma mark Scroll Wheel

- (void) scrollWheel:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ scrollWheel: %@", self, theEvent);
    self.inputAdapter->scrollWheel([self ogreWindow], theEvent);
}



@end
