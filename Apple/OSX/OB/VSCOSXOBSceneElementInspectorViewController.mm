//
//  VSCOSXOBSceneElementInspectorViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementInspectorViewController.h"

#import "VSCOSXEnvironmentController.h"
#import "VSCIMOSXCollisionEventChainController.h"
#import "VSCIMOSXCollisionEventChainViewController.h"
#import "VSCOSXOBSceneElementEditor.h"
#import "VSCOSXOBSceneElementDetailView.h"
#import "VSCOSXOBSceneElementCollisionView.h"
#import "DMTabBar.h"

#include "VSCIMCollisionEventChain.h"

#include <boost/assert.hpp>

NSString* const VSCOSXTabTitleElementDetails = @"Details";
NSString* const VSCOSXTabTitleElementCollision = @"Collision";

NSArray* ElementInspectorTabParamArray = nil;

@interface VSCOSXOBSceneElementInspectorViewController ()

@property (weak) IBOutlet DMTabBar* tabBar;
@property (weak) IBOutlet NSBox* mainBox;

@property (nonatomic, assign) VSC::OB::OSXSceneListener::SPtr sceneListener;

-(void) setupTabBar;
-(void) updateCollisionEventChains;

@end

@implementation VSCOSXOBSceneElementInspectorViewController

@synthesize element = _element;
@synthesize environmentController = _environmentController;

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
        
        self.collisionStartedEventChainViewController = [[VSCIMOSXCollisionEventChainViewController alloc]
                                                         initWithNibName:@"VSCIMOSXCollisionEventChainViewController"
                                                         bundle:nil];
        BOOST_ASSERT(self.collisionStartedEventChainViewController);
        self.collisionStartedEventChainViewController.elementController = self;
        
        self.collisionEndedEventChainViewController = [[VSCIMOSXCollisionEventChainViewController alloc]
                                                       initWithNibName:@"VSCIMOSXCollisionEventChainViewController"
                                                       bundle:nil];
        BOOST_ASSERT(self.collisionEndedEventChainViewController);
        self.collisionEndedEventChainViewController.elementController = self;
    }
    
    return self;
}

- (void) awakeFromNib
{
    BOOST_ASSERT(self.elementDetailView);
    BOOST_ASSERT(self.elementDetailView.elementController == self);
    [self.elementDetailView reloadWholeInterface];
    
    BOOST_ASSERT(self.elementCollisionView);
    BOOST_ASSERT(self.elementCollisionView.elementController == self);
    BOOST_ASSERT(self.elementCollisionView.collisionStartedEventChainBox);
    if (self.elementCollisionView.collisionStartedEventChainBox)
    {
        [self.elementCollisionView.collisionStartedEventChainBox setContentView:self.collisionStartedEventChainViewController.view];
        [self.collisionStartedEventChainViewController reloadInterface];
    }
    if (self.elementCollisionView.collisionEndedEventChainBox)
    {
        [self.elementCollisionView.collisionEndedEventChainBox setContentView:self.collisionEndedEventChainViewController.view];
        [self.collisionEndedEventChainViewController reloadInterface];
    }
    
    [self showElementDetailView];
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
    
    [self.elementDetailView reloadWholeInterface];
    
    [self updateCollisionEventChains];
    
}

-(void) updateCollisionEventChains
{
    VSC::OB::Scene::Element::SPtr elem = self.element.lock();
    
    VSC::IM::CollisionEventChain::SPtr collisionStartedEventChain = VSC::IM::CollisionEventChain::SPtr();
    VSC::IM::CollisionEventChain::SPtr collisionEndedEventChain = VSC::IM::CollisionEventChain::SPtr();
    
    if (elem)
    {
        collisionStartedEventChain = [self.environmentController collisionStartedEventChainForElement:elem];
        collisionEndedEventChain = [self.environmentController collisionEndedEventChainForElement:elem];
    }
    
    self.collisionStartedEventChainViewController.eventChain = collisionStartedEventChain;
    self.collisionEndedEventChainViewController.eventChain = collisionEndedEventChain;
    
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
            NSLog(@"%@ will select %lu/%@", self.tabBar, tabBarItemIndex, tabBarItem);
            if ([tabBarItem.toolTip isEqualToString:VSCOSXTabTitleElementDetails])
            {
                NSLog(@"Selected scene element list tab");
                [self showElementDetailView];
            }
            else if ([tabBarItem.toolTip isEqualToString:VSCOSXTabTitleElementCollision])
            {
                NSLog(@"Selected scene detail tab");
                [self showElementCollisionView];
            }
        }
        else if (selectionType == DMTabBarItemSelectionType_DidSelect)
        {
            NSLog(@"%@ did select %lu/%@", self.tabBar, tabBarItemIndex, tabBarItem);
        }
        
    }];
}


-(void) showElementDetailView
{
    self.tabBar.selectedIndex = 0;
    [self.mainBox setContentView:self.elementDetailView];
}

-(void) showElementCollisionView
{
    BOOST_ASSERT(self.elementCollisionView);
    self.tabBar.selectedIndex = 1;
    [self.mainBox setContentView:self.elementCollisionView];
    
}

#pragma mark - VSCOBOSXSceneListenerTarget

-(void) sceneWasRendered:(VSC::OB::Scene::SPtr)scene
{
    [self.elementDetailView reloadPositionInterface:NO];
}


@end
