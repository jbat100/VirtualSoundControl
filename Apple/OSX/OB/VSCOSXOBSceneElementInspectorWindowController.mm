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
#import "VSCOSXOBSceneElementCollisionView.h"

#import "VSCIMOSXCollisionActionView.h"
#import "VSCIMOSXDelayView.h"
#import "VSCIMOSXCollisionMappingListView.h"
#import "VSCIMOSXCollisionMappingView.h"
#import "VSCIMOSXCollisionEventChainView.h"

#import "PXListView.h"

#include "VSCIMCollisionMapper.h"
#include "VSCIMEvent.h"
#include "VSCIMDelay.h"
#include "VSCIMAction.h"
#include "VSCIMCollisionMIDIActions.h"

#include <boost/assert.hpp>

NSString* const VSCIMOSXCollisionMappingViewReuseIdentifier     = @"VSCIMOSXCollisionMappingViewReuseIdentifier";
NSString* const VSCIMOSXCollisionActionViewReuseIdentifier      = @"VSCIMOSXCollisionActionViewReuseIdentifier";
NSString* const VSCIMOSXDelayViewReuseIdentifier                = @"VSCIMOSXDelayViewReuseIdentifier";

@interface VSCOSXOBSceneElementInspectorWindowController ()

-(VSC::IM::CollisionEventChain::SPtr) collisionEventChainForEditor:(id<VSCIMOSXCollisionEventChainEditor>)editor;

-(VSCIMOSXCollisionMappingView*) newCollisionMappingView;
-(VSCIMOSXCollisionActionView*) newCollisionActionView;
-(VSCIMOSXDelayView*) newDelayView;

@end

@implementation VSCOSXOBSceneElementInspectorWindowController

@synthesize element = _element;
@synthesize environmentController = _environmentController;


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


-(void) setElement:(VSC::OB::Scene::Element::WPtr)element
{
    
    if (element.lock() == _element.lock()) return;
    
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

#pragma mark - VSCOBOSXSceneListenerTarget

-(void) sceneWasRendered:(VSC::OB::Scene::SPtr)scene
{
    [self.elementInspectorView.elementDetailView reloadPositionInterface:NO];
}

#pragma mark - VSCOSXOBSceneElementController

-(VSC::IM::CollisionEventChain::SPtr) collisionEventChainForEditor:(id<VSCIMOSXCollisionEventChainEditor>)editor
{
    BOOST_ASSERT([editor isKindOfClass:[VSCIMOSXCollisionEventChainView class]]);
    if ([editor isKindOfClass:[VSCIMOSXCollisionEventChainView class]] == NO) return VSC::IM::CollisionEventChain::SPtr();
    return [(VSCIMOSXCollisionEventChainView*)editor eventChain].lock();
}

-(void) collisionEventChainEditor:(id<VSCIMOSXCollisionEventChainEditor>)editor requestsAppendingEvent:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = [self collisionEventChainForEditor:editor];
    
    BOOST_ASSERT(chain);
    BOOST_ASSERT(event);
    
    if (chain && event)
    {
        chain->appendEvent(event);
        [(VSCOSXOBSceneElementCollisionView*)editor reloadInterface];
    }
}

-(void) collisionEventChainEditor:(id<VSCIMOSXCollisionEventChainEditor>)editor requestsRemovingEvent:(VSC::IM::Event::SPtr)event
{
    VSC::IM::CollisionEventChain::SPtr chain = [self collisionEventChainForEditor:editor];
    
    BOOST_ASSERT(chain);
    BOOST_ASSERT(event);
    
    if (chain && event)
    {
        chain->removeEvent(event);
        [(VSCOSXOBSceneElementCollisionView*)editor reloadInterface];
    }
    
}


#pragma mark - PXListViewDelegate

- (NSUInteger)numberOfRowsInListView:(PXListView*)aListView
{
    if ([[aListView superview] isKindOfClass:[VSCIMOSXCollisionEventChainView class]])
    {
        VSCIMOSXCollisionEventChainView* eventChainView = (VSCIMOSXCollisionEventChainView*)[aListView superview];
        VSC::IM::CollisionEventChain::SPtr eventChain = [eventChainView eventChain].lock();
        BOOST_ASSERT(eventChain);
        if (eventChain) eventChain->getNumberOfEvents();
    }
    
    else if ([aListView isKindOfClass:[VSCIMOSXCollisionMappingListView class]])
    {
        VSCIMOSXCollisionMappingListView* mappingListView = (VSCIMOSXCollisionMappingListView*)[aListView superview];
        VSC::IM::CollisionAction::SPtr collisionAction = [mappingListView action].lock();
        BOOST_ASSERT(collisionAction);
        if (collisionAction) collisionAction->getExpectedMappingTargets().size();
    }
    
	return 0;
}

- (PXListViewCell*)listView:(PXListView*)aListView cellForRow:(NSUInteger)row
{
    if ([[aListView superview] isKindOfClass:[VSCIMOSXCollisionEventChainView class]])
    {
        VSCIMOSXCollisionEventChainView* eventChainView = (VSCIMOSXCollisionEventChainView*)[aListView superview];
        VSC::IM::CollisionEventChain::SPtr eventChain = [eventChainView eventChain].lock();
        BOOST_ASSERT(eventChain);
        VSC::IM::Event::SPtr event = VSC::IM::Event::SPtr();
        if (eventChain) event = eventChain->getEventAtIndex((unsigned int)row);
        
        VSC::IM::CollisionAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionAction>(event);
        if (action)
        {
            PXListViewCell* cell = [aListView dequeueCellWithReusableIdentifier:VSCIMOSXCollisionActionViewReuseIdentifier];
            VSCIMOSXCollisionActionView* actionView = nil;
            if (cell)
            {
                BOOST_ASSERT([cell isKindOfClass:[VSCIMOSXCollisionActionView class]]);
                actionView = (VSCIMOSXCollisionActionView*)cell;
            }
            if(!actionView)
            {
                actionView = [self newCollisionActionView];
            }
            [actionView setCollisionAction:(VSC::IM::CollisionAction::WPtr(action))];
            actionView.eventChainEditor = eventChainView;
            return actionView;
        }
        
        VSC::IM::Delay::SPtr delay = boost::dynamic_pointer_cast<VSC::IM::Delay>(event);
        if (delay)
        {
            PXListViewCell* cell = [aListView dequeueCellWithReusableIdentifier:VSCIMOSXDelayViewReuseIdentifier];
            VSCIMOSXDelayView* delayView = nil;
            if (cell)
            {
                BOOST_ASSERT([cell isKindOfClass:[VSCIMOSXDelayView class]]);
                delayView = (VSCIMOSXDelayView*)cell;
            }
            if(!delayView)
            {
                delayView = [self newDelayView];
            }
            [delayView setDelay:(VSC::IM::Delay::WPtr(delay))];
            return delayView;
        }
        
    }
    
    else if ([aListView isKindOfClass:[VSCIMOSXCollisionMappingListView class]])
    {
        VSCIMOSXCollisionMappingListView* mappingListView = (VSCIMOSXCollisionMappingListView*)[aListView superview];
        VSC::IM::CollisionAction::SPtr collisionAction = [mappingListView action].lock();
        BOOST_ASSERT(collisionAction);
        
        if (collisionAction)
        {
            const VSC::IM::Targets& targets = collisionAction->getExpectedMappingTargets();
            BOOST_ASSERT(targets.size() > row);
            if (targets.size() <= row) return nil;
            VSC::IM::Target target = targets.at(row);
            VSC::IM::CollisionMapping::SPtr collisionMapping = collisionAction->getMappingForTarget(target);
            
            if (collisionMapping)
            {
                PXListViewCell* cell = [aListView dequeueCellWithReusableIdentifier:VSCIMOSXCollisionMappingViewReuseIdentifier];
                VSCIMOSXCollisionMappingView* mappingView = nil;
                
                if (cell)
                {
                    BOOST_ASSERT([cell isKindOfClass:[VSCIMOSXCollisionMappingView class]]);
                    mappingView = (VSCIMOSXCollisionMappingView*)cell;
                }
                
                if(!mappingView)
                {
                    mappingView = [self newCollisionMappingView];
                }
                
                [mappingView setMapping:(VSC::IM::CollisionMapping::WPtr(collisionMapping))];
                
                return mappingView;
            }
        }
    }
	
	return nil;
}

- (CGFloat)listView:(PXListView*)aListView heightOfRow:(NSUInteger)row
{
    if ([[aListView superview] isKindOfClass:[VSCIMOSXCollisionEventChainView class]])
    {
        VSCIMOSXCollisionEventChainView* eventChainView = (VSCIMOSXCollisionEventChainView*)[aListView superview];
        VSC::IM::CollisionEventChain::SPtr eventChain = [eventChainView eventChain].lock();
        BOOST_ASSERT(eventChain);
        VSC::IM::Event::SPtr event = VSC::IM::Event::SPtr();
        if (eventChain) event = eventChain->getEventAtIndex((unsigned int)row);
        
        VSC::IM::CollisionAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionAction>(event);
        if (action) return [VSCIMOSXCollisionActionView heightOfViewForCollisionAction:action];
        
        VSC::IM::Delay::SPtr delay = boost::dynamic_pointer_cast<VSC::IM::Delay>(event);
        if (delay) return [VSCIMOSXDelayView defaultViewHeight];
        
        BOOST_ASSERT_MSG(false, "Expected CollisionAction or Delay");
    }
    
    else if ([aListView isKindOfClass:[VSCIMOSXCollisionMappingListView class]])
    {
        VSCIMOSXCollisionMappingListView* mappingListView = (VSCIMOSXCollisionMappingListView*)[aListView superview];
        VSC::IM::CollisionAction::SPtr collisionAction = [mappingListView action].lock();
        BOOST_ASSERT(collisionAction);
        
        if (collisionAction)
        {
            const VSC::IM::Targets& targets = collisionAction->getExpectedMappingTargets();
            BOOST_ASSERT(targets.size() > row);
            if (targets.size() <= row) return 0;
            VSC::IM::Target target = targets.at(row);
            VSC::IM::CollisionMapping::SPtr collisionMapping = collisionAction->getMappingForTarget(target);
            
            return [VSCIMOSXCollisionMappingView heightOfViewForCollisionMapping:collisionMapping];
        }
    }
    
    BOOST_ASSERT_MSG(false, "Shouldn't reach this point");
    
    return 0;
}

- (void)listViewSelectionDidChange:(NSNotification*)aNotification
{
    NSLog(@"Selection changed");
}

#pragma mark - View Factory Methods

-(VSCIMOSXCollisionMappingView*) newCollisionMappingView
{
    VSCIMOSXCollisionMappingView* cell = [VSCIMOSXCollisionMappingView cellLoadedFromNibNamed:@"VSCIMOSXCollisionMappingView"
                                                                                       bundle:nil
                                                                           reusableIdentifier:VSCIMOSXCollisionMappingViewReuseIdentifier];
    
    BOOST_ASSERT(cell);
    
    return cell;
}

-(VSCIMOSXCollisionActionView*) newCollisionActionView
{
    VSCIMOSXCollisionActionView* cell = [VSCIMOSXCollisionActionView cellLoadedFromNibNamed:@"VSCIMOSXCollisionActionView"
                                                                                     bundle:nil
                                                                         reusableIdentifier:VSCIMOSXCollisionActionViewReuseIdentifier];
    
    BOOST_ASSERT(cell);
    
    return cell;
}

-(VSCIMOSXDelayView*) newDelayView
{
    VSCIMOSXDelayView* cell = [VSCIMOSXDelayView cellLoadedFromNibNamed:@"VSCIMOSXDelayView"
                                                                 bundle:nil
                                                     reusableIdentifier:VSCIMOSXDelayViewReuseIdentifier];
    
    BOOST_ASSERT(cell);
    
    return cell;
}




@end
