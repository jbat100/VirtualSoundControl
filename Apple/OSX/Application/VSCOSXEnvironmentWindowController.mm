//
//  VSCOBOSXSceneWindow.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXEnvironmentWindowController.h"
#import "VSCOSXApplicationManager.h"
#import "VSCOSXOBSceneController.h"
#import "VSCOBOSXSceneDisplayView.h"
#import "VSCOSXEnvironmentInspectorView.h"
#import "VSCOSXOBSceneElementInspectorWindowController.h"
#import "VSCOSXOBSceneElementInspectorViewController.h"

#import "NSString+VSCAdditions.h"
#import "NSArray+VSCAdditions.h"

#include "VSCEnvironment.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCIMCollisionMapper.h"

#include <Ogre/Ogre.h>

#include <vector>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

@interface VSCOSXEnvironmentWindowController ()

-(void) reloadSceneInterface;

@end

@implementation VSCOSXEnvironmentWindowController

@synthesize environment = _environment;
@synthesize sceneController = _sceneController;

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self)
    {
        // Initialization code here.
    }
    
    return self;
}

-(id) initWithWindowNibName:(NSString *)windowNibName
{
    self = [super initWithWindowNibName:windowNibName];
    if (self)
    {
        
    }
    
    return self;
}

-(void) dealoc
{
    NSLog(@"%@ DEALLOC", self);
}

-(void) windowDidLoad
{
    [super windowDidLoad];
    
    NSLog(@"%@ windowDidLoad owner: %@, windowNibName: %@", self, [self owner], [self windowNibName]);
    
    BOOST_ASSERT(self.sceneController);
    BOOST_ASSERT(self.environmentInspectorView);
}

-(void) windowWillLoad
{
    
}


-(BOOL)acceptsFirstResponder {
    return NO;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
    return NO;
}

#pragma mark - Custom Setters

- (void) setEnvironment:(VSC::Environment::WPtr)environment
{
    _environment = environment;
    [self reloadInterface];
    
}


-(void) reloadInterface
{
    VSC::Environment::SPtr env = self.environment.lock();
    
    [self reloadSceneInterface];
}

-(void) reloadSceneInterface
{
    VSC::Environment::SPtr env = self.environment.lock();
    
    if (env)
    {
        self.sceneController.scene = VSC::OB::Scene::WPtr(env->getOBScene());
    }
    else
    {
        self.sceneController.scene = VSC::OB::Scene::WPtr();
    }
}

-(void) showSceneDetails
{
    BOOST_ASSERT(self.environmentInspectorView);
    
    [self.environmentInspectorView showSceneDetail];
}

-(void) showSceneElementList
{
    BOOST_ASSERT(self.environmentInspectorView);
    
    [self.environmentInspectorView showSceneElementList];
}

-(void) showElementInspectorForElement:(VSC::OB::Scene::Element::SPtr)element
{
    NSLog(@"%@ showElementInspectorForElement", self);
    
    if (!self.elementInspectorWindowController)
    {
        NSString* nibName = @"VSCOSXOBSceneElementInspectorWindowController";
        self.elementInspectorWindowController = [[VSCOSXOBSceneElementInspectorWindowController alloc] initWithWindowNibName:nibName];
        BOOST_ASSERT(self.elementInspectorWindowController);
    }
    
    BOOST_ASSERT(self.elementInspectorWindowController.elementInspectorViewController);
    self.elementInspectorWindowController.elementInspectorViewController.element = VSC::OB::Scene::Element::WPtr(element);
    [self.elementInspectorWindowController showWindow:self];
    [self.elementInspectorWindowController.elementInspectorViewController showElementDetailView];
}

-(VSC::IM::CollisionEventChain::SPtr) collisionStartedEventChainForElement:(VSC::OB::Scene::Element::SPtr)element
{
    VSC::Environment::SPtr env = self.environment.lock();
    BOOST_ASSERT(env);
    
    if (env)
    {
        return env->getIMCollisionMapper()->getActionChainForCollisionStarted(element);
    }
    
    return VSC::IM::CollisionEventChain::SPtr();
}

-(VSC::IM::CollisionEventChain::SPtr) collisionEndedEventChainForElement:(VSC::OB::Scene::Element::SPtr)element
{
    VSC::Environment::SPtr env = self.environment.lock();
    BOOST_ASSERT(env);
    
    if (env)
    {
        return env->getIMCollisionMapper()->getActionChainForCollisionEnded(element);
    }
    
    return VSC::IM::CollisionEventChain::SPtr();
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
