//
//  VSCOSXOBSceneElementInspectorViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementInspectorViewController.h"

#import "VSCOSXEnvironmentController.h"
#import "VSCIMOSXEventChainController.h"
#import "VSCIMOSXEventChainViewController.h"
#import "VSCOSXOBSceneElementEditor.h"
#import "VSCOSXOBSceneElementDetailView.h"
#import "VSCOSXOBSceneElementCollisionView.h"
#import "DMTabBar.h"

#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCIMEventChain.h"

#include <boost/assert.hpp>

NSString* const VSCOSXTabTitleElementDetails = @"Details";
NSString* const VSCOSXTabTitleElementCollision = @"Collision";

NSArray* ElementInspectorTabParamArray = nil;

@interface VSCOSXOBSceneElementInspectorViewController ()

@property (nonatomic, strong) NSArray* tabViewConstraints;

@property (nonatomic, assign) VSC::OB::OSXSceneListener::SPtr sceneListener;

-(void) setupTabBar;
-(void) resetInspectorView;
-(void) updateEventChains;

@end

@implementation VSCOSXOBSceneElementInspectorViewController

const static BOOL traceInterface = YES;

@synthesize element = _element;
@synthesize environmentController = _environmentController;
@synthesize immobilized;

+ (void)load
{
    ElementInspectorTabParamArray = @[
    @{@"image": [NSImage imageNamed:@"19-gear.png"], @"title": VSCOSXTabTitleElementDetails},
    @{@"image": [NSImage imageNamed:@"55-network.png"], @"title": VSCOSXTabTitleElementCollision},
    ];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        self.sceneListener = VSC::OB::OSXSceneListener::SPtr (new VSC::OB::OSXSceneListener);
        BOOST_ASSERT(self.sceneListener);
        if (self.sceneListener) self.sceneListener->setTarget(self);
        
        self.collisionStartedEventChainViewController = [[VSCIMOSXEventChainViewController alloc]
                                                         initWithNibName:@"VSCIMOSXEventChainViewController"
                                                         bundle:nil];
        BOOST_ASSERT(self.collisionStartedEventChainViewController);
        self.collisionStartedEventChainViewController.elementController = self;
        
        self.collisionEndedEventChainViewController = [[VSCIMOSXEventChainViewController alloc]
                                                       initWithNibName:@"VSCIMOSXEventChainViewController"
                                                       bundle:nil];
        BOOST_ASSERT(self.collisionEndedEventChainViewController);
        self.collisionEndedEventChainViewController.elementController = self;
    }
    
    return self;
}

- (void) awakeFromNib
{
    BOOST_ASSERT(self.view);
    BOOST_ASSERT(self.tabBar);
    BOOST_ASSERT(self.elementCollisionView);
    BOOST_ASSERT(self.elementCollisionView.elementController == self);
    BOOST_ASSERT(self.elementDetailView);
    BOOST_ASSERT(self.elementDetailView.elementController == self);
    
    self.view.translatesAutoresizingMaskIntoConstraints = NO;
    self.tabBar.translatesAutoresizingMaskIntoConstraints = NO;
    self.elementCollisionView.translatesAutoresizingMaskIntoConstraints = NO;
    self.elementDetailView.translatesAutoresizingMaskIntoConstraints = NO;
    
    
    [self setupTabBar];
    
    [self.elementDetailView reloadWholeInterface];
    
    if (self.elementCollisionView)
    {
        NSView* eventChainView = self.collisionStartedEventChainViewController.view;
        
        BOOST_ASSERT(eventChainView);
        
        [self.elementCollisionView addSubview:eventChainView];
        
        NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(eventChainView);
        [self.elementCollisionView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[eventChainView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
        [self.elementCollisionView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[eventChainView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
        
        [self.collisionStartedEventChainViewController reloadInterface];
    }
    
    [self showElementDetailView];
}

-(void) setElement:(VSC::OB::Element::WPtr)element
{
    
    if (element.lock() == _element.lock()) return;
    
    // stop listening to the scene of the old element
    VSC::OB::Element::SPtr oldElement = _element.lock();
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
    VSC::OB::Element::SPtr newElement = _element.lock();
    if (newElement)
    {
        VSC::OB::Scene::SPtr scene = newElement->getScene();
        if (scene)
        {
            BOOST_ASSERT(self.sceneListener);
            scene->addListener(boost::dynamic_pointer_cast<VSC::Listener>(self.sceneListener));
        }
    }
    
    if (traceInterface) NSLog(@"%@ setElement %p", self, newElement.get());
    
    [self reloadImmobilizedInterface];
    
    [self.elementDetailView reloadWholeInterface];
    
    [self updateEventChains];
    
}

-(void) setImmobilized:(BOOL)immobilized
{
    VSC::OB::Element::SPtr element = self.element.lock();
    if (element)
    {
        element->setImmobilized(immobilized ? true : false);
    }
}

-(void) updateEventChains
{
    VSC::OB::Element::SPtr elem = self.element.lock();
    
    VSC::IM::EventChain::SPtr collisionStartedEventChain = VSC::IM::EventChain::SPtr();
    VSC::IM::EventChain::SPtr collisionEndedEventChain = VSC::IM::EventChain::SPtr();
    
    if (elem)
    {
        BOOST_ASSERT(self.environmentController);
        
        collisionStartedEventChain = [self.environmentController collisionStartedEventChainForElement:elem];
        collisionEndedEventChain = [self.environmentController collisionEndedEventChainForElement:elem];
    }
    
    self.collisionStartedEventChainViewController.eventChain = collisionStartedEventChain;
    self.collisionEndedEventChainViewController.eventChain = collisionEndedEventChain;
    
}

-(void) reloadImmobilizedInterface
{
    VSC::OB::Element::SPtr element = self.element.lock();
    
    if (element)
    {
        self.immobilized = element->isImmobilized() ? YES : NO;
    }
}

#pragma mark - UI Helpers

-(void) setupTabBar
{
    BOOST_ASSERT(ElementInspectorTabParamArray);
    
    NSMutableArray *items = [[NSMutableArray alloc] initWithCapacity:[ElementInspectorTabParamArray count]];
    
    [ElementInspectorTabParamArray enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        
        BOOST_ASSERT([obj isKindOfClass:[NSDictionary class]]);
        if (![obj isKindOfClass:[NSDictionary class]]) return;
        
        NSDictionary* objDict = (NSDictionary*)obj;
        NSImage *iconImage = [objDict objectForKey:@"image"];
        [iconImage setTemplate:YES];
        
        DMTabBarItem *item = [DMTabBarItem tabBarItemWithIcon:iconImage tag:idx];
        item.toolTip = [objDict objectForKey:@"title"];
        item.keyEquivalent = [NSString stringWithFormat:@"%ld",idx];
        item.keyEquivalentModifierMask = NSCommandKeyMask;
        [items addObject:item];
        
    }];
    
    // Load them
    self.tabBar.tabBarItems = items;
    
    // Handle selection events
    [self.tabBar handleTabBarItemSelection:^(DMTabBarItemSelectionType selectionType, DMTabBarItem *tabBarItem, NSUInteger tabBarItemIndex) {
        
        if (selectionType == DMTabBarItemSelectionType_WillSelect)
        {
            if (traceInterface) NSLog(@"%@ will select %lu/%@", self.tabBar, tabBarItemIndex, tabBarItem);
            if ([tabBarItem.toolTip isEqualToString:VSCOSXTabTitleElementDetails])
            {
                if (traceInterface) NSLog(@"Selected scene element list tab");
                [self showElementDetailView];
            }
            else if ([tabBarItem.toolTip isEqualToString:VSCOSXTabTitleElementCollision])
            {
                if (traceInterface) NSLog(@"Selected scene detail tab");
                [self showElementCollisionView];
            }
        }
        else if (selectionType == DMTabBarItemSelectionType_DidSelect)
        {
            if (traceInterface) NSLog(@"%@ did select %lu/%@", self.tabBar, tabBarItemIndex, tabBarItem);
        }
        
    }];
}

-(void) resetInspectorView
{
    NSArray* subviews = self.view.subviews;
    
    for (NSView* v in subviews)
    {
        BOOST_ASSERT([v isKindOfClass:[NSView class]]);
        if (v != self.tabBar && [v isKindOfClass:[NSView class]])
        {
            [v removeFromSuperview];
        }
    }
}

-(void) switchElementInspectorToTabView:(NSView*)tabView
{
    if (tabView == nil) return;
    
    BOOST_ASSERT(tabView);
    BOOST_ASSERT([tabView isKindOfClass:[NSView class]]);
    
    if ([tabView isKindOfClass:[NSView class]] == NO) return;
    if ([tabView superview] == self.view) return;
    
    if (self.tabViewConstraints)
    {
        [self.view removeConstraints:self.tabViewConstraints];
    }
    
    [self resetInspectorView];
    
    [self.view addSubview:tabView];
    
    NSView* bar = self.tabBar;
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(tabView, bar);
    
    NSMutableArray* allConstraints = [NSMutableArray array];
    
    NSArray* hConstraints =[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[tabView]|"
                                                                   options:0
                                                                   metrics:nil
                                                                     views:viewsDictionary];
    
    [allConstraints addObjectsFromArray:hConstraints];
    [self.view addConstraints:hConstraints];
    
    NSArray* vConstraints =[NSLayoutConstraint constraintsWithVisualFormat:@"V:[bar]-0-[tabView]|"
                                                                   options:0
                                                                   metrics:nil
                                                                     views:viewsDictionary];
    
    [allConstraints addObjectsFromArray:vConstraints];
    [self.view addConstraints:vConstraints];
    
    self.tabViewConstraints = [NSArray arrayWithArray:allConstraints];
}


-(void) showElementDetailView
{
    if (traceInterface) NSLog(@"%@ showElementDetailView", self);
    
    BOOST_ASSERT(self.elementDetailView);

    [self switchElementInspectorToTabView:self.elementDetailView];
}

-(void) showElementCollisionView
{
    if (traceInterface) NSLog(@"%@ showElementCollisionView", self);
    
    BOOST_ASSERT(self.elementCollisionView);
    
    [self switchElementInspectorToTabView:self.elementCollisionView];
    
    [self updateEventChains];
    
}

#pragma mark - VSCOBOSXSceneListenerTarget

-(void) sceneWasRendered:(VSC::OB::Scene::SPtr)scene
{
    [self.elementDetailView reloadPositionInterface:NO];
}


@end
