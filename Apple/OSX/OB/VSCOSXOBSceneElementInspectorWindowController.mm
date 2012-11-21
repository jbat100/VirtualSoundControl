//
//  VSCSceneElementInspectorWindowController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementInspectorWindowController.h"
#import "VSCOSXOBSceneElementController.h"
#import "VSCOSXOBSceneElementInspectorView.h"
#import "VSCOSXOBSceneElementDetailView.h"

@interface VSCOSXOBSceneElementInspectorWindowController ()

@end

@implementation VSCOSXOBSceneElementInspectorWindowController

@synthesize element = _element;
@synthesize environmentController = _environmentController;
@synthesize collisionEventChainController = _collisionEventChainController;

// according to the documentation, is the designated initializer so should always be called?
- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        // Initialization code here.
        
        self.sceneListener = VSC::OB::OSXSceneListener::SPtr (new VSC::OB::OSXSceneListener);
        BOOST_ASSERT(self.sceneListener);
        if (self.sceneListener) self.sceneListener->setTarget(self);
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

-(void) showElementDetailView
{
    [self.elementInspectorView showElementDetailView];
}

-(void) showElementCollisionView
{
    [self.elementInspectorView showElementCollisionView];
}

-(void) sceneWasRendered
{
    [self.elementInspectorView.elementDetailView reloadInterface];
}

-(void) setElement:(VSC::OB::Scene::Element::WPtr)element
{
    
    
     
    // stop listening to the scene of the old element
    VSC::OB::Scene::Element::SPtr oldElement = _element.lock();
    if (oldElement)
    {
        VSC::OB::Scene::SPtr scene = oldElement->getScene();
        if (scene)
        {
            BOOST_ASSERT(self.sceneListener);
            scene->removeListener(boost::dynamic_pointer_cast<VSC::Listener>(self.sceneListener));
        }
    }
    
    
    
    _element = element;
    
    
    
    // start listening to the scene of the old element
    VSC::OB::Scene::Element::SPtr newElement = _element.lock();
    if (newElement)
    {
        VSC::OB::Scene::SPtr scene = newElement->getScene();
        if (scene)
        {
            BOOST_ASSERT(self.sceneListener);
            scene->addListener(boost::dynamic_pointer_cast<VSC::Listener>(self.sceneListener));
        }
    }
     
     
}

-(void) sceneWasRendered:(VSC::OB::Scene::SPtr)scene
{
    [self.elementInspectorView.elementDetailView reloadInterface];
}

@end
