//
//  VSCOBOSXElementInspectorViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOBOSXElementInspectorViewController.h"

#import "VSCOSXEnvironmentController.h"

#import "VSCIMOSXEventChainController.h"
#import "VSCIMOSXEventChainViewController.h"
#import "VSCOBOSXElementEditor.h"
#import "VSCOBOSXElementDetailView.h"
#import "VSCOBOSXElementEventChainsView.h"
#import "DMTabBar.h"
#import "NSString+VSCAdditions.h"

#include "VSCEnvironment.h"
#include "VSCCollisionMapper.h"
#include "VSCOB.h"
#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCIM.h"
#include "VSCIMEventChain.h"

#include <boost/assert.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

NSString* const VSCOSXTabTitleElementDetails = @"Details";
NSString* const VSCOSXTabTitleElementCollision = @"Collision";

NSArray* ElementInspectorTabParamArray = nil;

@interface VSCOBOSXElementInspectorViewController ()

@property (nonatomic, strong) NSArray* tabViewConstraints;

@property (nonatomic, assign) OSXSceneListener::SPtr sceneListener;

-(void) setupTabBar;
-(void) resetInspectorView;

@end

@implementation VSCOBOSXElementInspectorViewController

const static BOOL traceInterface = YES;

@synthesize element = _element;
@synthesize environmentController = _environmentController;
@synthesize immobilized = _immobilized;

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
        self.sceneListener = OSXSceneListener::SPtr (new OSXSceneListener);
        BOOST_ASSERT(self.sceneListener);
        if (self.sceneListener) self.sceneListener->setTarget(self);
    }
    
    return self;
}

-(void) setupIfNecessary
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        
        BOOST_ASSERT(self.view);
        BOOST_ASSERT(self.tabBar);
        BOOST_ASSERT(self.elementEventChainsView);
        BOOST_ASSERT(self.elementEventChainsView.elementController == self);
        BOOST_ASSERT(self.elementDetailView);
        BOOST_ASSERT(self.elementDetailView.elementController == self);
        
        BOOST_ASSERT([self.elementDetailView isKindOfClass:[VSCOBOSXElementDetailView class]]);
        
        NSLog(@"self.elementDetailView %@", self.elementDetailView);
        
        self.view.translatesAutoresizingMaskIntoConstraints = NO;
        self.tabBar.translatesAutoresizingMaskIntoConstraints = NO;
        self.elementEventChainsView.translatesAutoresizingMaskIntoConstraints = NO;
        self.elementDetailView.translatesAutoresizingMaskIntoConstraints = NO;
        
        [self setupTabBar];
        
        [self.elementDetailView setupIfNecessary];
        
        [self showElementDetailView];
        
    });
}

- (void) awakeFromNib
{

}

-(void) setElement:(Element::WPtr)element
{
    
    if (element.lock() == _element.lock()) return;
    
    // stop listening to the scene of the old element
    Element::SPtr oldElement = _element.lock();
    if (oldElement)
    {
        Scene::SPtr scene = oldElement->getScene();
        if (scene)
        {
            BOOST_ASSERT(self.sceneListener);
            scene->removeListener(boost::dynamic_pointer_cast<Listener>(self.sceneListener));
        }
    }
    
    _element = element;
    
    // start listening to the scene of the old element
    Element::SPtr newElement = _element.lock();
    if (newElement)
    {
        Scene::SPtr scene = newElement->getScene();
        if (scene)
        {
            BOOST_ASSERT(self.sceneListener);
            scene->addListener(boost::dynamic_pointer_cast<Listener>(self.sceneListener));
        }
    }
    
    if (traceInterface) NSLog(@"%@ setElement %p", self, newElement.get());
    
    [self reloadImmobilizedInterface];
    
    [self.elementDetailView reloadWholeInterface];
    
}

-(void) setImmobilized:(BOOL)immob
{
    Element::SPtr element = self.element.lock();
    if (element)
    {
        element->setImmobilized(immob ? true : false);
    }
    _immobilized = immob;
}


-(void) reloadImmobilizedInterface
{
    Element::SPtr element = self.element.lock();
    
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
                [self showElementEventChainsView];
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
    
    [self.elementDetailView setupIfNecessary];
    
    [self.elementDetailView reloadWholeInterface];

    [self switchElementInspectorToTabView:self.elementDetailView];
}

-(void) showElementEventChainsView
{
    if (traceInterface) NSLog(@"%@ showElementEventChainsView", self);
    
    BOOST_ASSERT(self.elementEventChainsView);
    
    //[self.elementEventChainsView setupIfNecessary];
    
    [self switchElementInspectorToTabView:self.elementEventChainsView];
}

#pragma mark - VSCOBOSXSceneListenerTarget

-(void) sceneWasRendered:(Scene::SPtr)scene
{
    if (self.elementDetailView)
    {
        //NSLog(@"self.elementDetailView %@", self.elementDetailView);
        BOOST_ASSERT([self.elementDetailView isKindOfClass:[VSCOBOSXElementDetailView class]]);
    }
    
    [self.elementDetailView reloadPositionInterface:NO];
}

#pragma mark - NSTableView Delegate/DataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    BOOST_ASSERT(self.environmentController);
    Environment::SPtr environment = self.environmentController.environment.lock();
    BOOST_ASSERT(environment);
    if (environment)
    {
        const EventChains& eventChains = environment->getEventChains();
        return eventChains.size();
    }
    
    return 0;
}


- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    BOOST_ASSERT(self.environmentController);
    Environment::SPtr environment = self.environmentController.environment.lock();
    BOOST_ASSERT(environment);
    if (environment)
    {
        const EventChains& eventChains = environment->getEventChains();
        EventChain::SPtr eventChain = EventChain::SPtr();
        BOOST_ASSERT(eventChains.size() > row);
        if (eventChains.size() > row)
        {
            eventChain = eventChains.at(row);
        }
        BOOST_ASSERT(eventChain);
        if (eventChain)
        {
            // Return the object property corresponding to the column
            if([[tableColumn identifier] isEqualToString:@"name"])
            {
                return [NSString stringWithStdString:eventChain->getUsername()];
            }
        }
        
        
        // Since this method has return type `id', we need to box the
        // boolean values inside an `NSNumber' instance
        else if([[tableColumn identifier] isEqualToString:@"linked"])
        {
            Element::SPtr elem = self.element.lock();
            BOOST_ASSERT(elem);
            CollisionMapper::SPtr collisionMapper = environment->getCollisionMapper();
            BOOST_ASSERT(collisionMapper);
            if (collisionMapper && elem)
            {
                /*
                 *  Check if the element is linked to the eventChain (via the environment collision mapper)
                 */
                
                const EventChains& linkedEventChains = collisionMapper->getEventChainsForCollisionStarted(elem);
                EventChains::const_iterator it = std::find(linkedEventChains.begin(), linkedEventChains.end(), eventChain);
                
                bool linked = it != linkedEventChains.end();
                
                return [NSNumber numberWithBool:linked ? YES : NO];
            }
        }
    }
    
    return nil;
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    /*
     *  Only the linked checkbox should be allowed to change, ensure this is the case
     */
    
    BOOST_ASSERT([[aTableColumn identifier] isEqualToString:@"linked"]);
    
    if ([[aTableColumn identifier] isEqualToString:@"linked"])
    {
        BOOST_ASSERT(self.environmentController);
        Environment::SPtr environment = self.environmentController.environment.lock();
        BOOST_ASSERT(environment);
        Element::SPtr elem = self.element.lock();
        BOOST_ASSERT(elem);
        
        if (environment && elem)
        {
            CollisionMapper::SPtr collisionMapper = environment->getCollisionMapper();
            BOOST_ASSERT(collisionMapper);
            
            if (collisionMapper)
            {
                const EventChains& eventChains = environment->getEventChains();
                BOOST_ASSERT(eventChains.size() > rowIndex);
                
                if (eventChains.size() > rowIndex)
                {
                    EventChain::SPtr eventChain = eventChains.at(rowIndex);
                    
                    BOOL link = [anObject boolValue];
                    if (link)
                    {
                        collisionMapper->addEventChainForCollisionStarted(eventChain, elem);
                    }
                    else
                    {
                        collisionMapper->removeEventChainForCollisionStarted(eventChain, elem);
                    }
                }
            }
            
        }
    }
}


@end
