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
#import "VSCOSXOBSceneElementInspectorWindowController.h"
#import "VSCOSXOBSceneElementInspectorViewController.h"
#import "VSCOSXEnvironmentController.h"
#import "VSCOSXOBSceneElementListView.h"
#import "VSCOSXOBSceneDetailView.h"

#import "NSString+VSCAdditions.h"
#import "NSArray+VSCAdditions.h"

#import "DMTabBar.h"

#include "VSCEnvironment.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCIMCollisionMapper.h"

#include <Ogre/Ogre.h>

#include <vector>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

NSArray* EnvironmentInspectorTabParamArray = nil;

NSString* const VSCOSXTabTitleSceneSettings = @"Scene Settings";
NSString* const VSCOSXTabTitleSceneElements = @"Scene Elements";
NSString* const VSCOSXTabTitleEnveloppes = @"Enveloppes";

@interface VSCOSXEnvironmentWindowController ()

@property (weak) IBOutlet DMTabBar* tabBar;

@property (strong) NSArray* tabViewConstraints;

-(void) customInit;
-(void) reloadSceneInterface;
-(void) setupTabBar;
-(void) setupConstraints;
-(void) setupTest;

-(void) resetInspectorView;
-(void) switchEnvironmentInspectorToTabView:(NSView*)tabView;

@end

@implementation VSCOSXEnvironmentWindowController

@synthesize environment = _environment;
@synthesize sceneController = _sceneController;

+(void) load {
    
    EnvironmentInspectorTabParamArray = @[
    @{@"image": [NSImage imageNamed:@"158-wrench-2.png"], @"title": VSCOSXTabTitleSceneSettings},
    @{@"image": [NSImage imageNamed:@"12-eye"], @"title": VSCOSXTabTitleSceneElements},
    @{@"image": [NSImage imageNamed:@"122-stats"], @"title": VSCOSXTabTitleEnveloppes}
    ];
    
}

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self)
    {
        [self customInit];
    }
    return self;
}

-(id) initWithWindowNibName:(NSString *)windowNibName
{
    self = [super initWithWindowNibName:windowNibName];
    if (self)
    {
        [self customInit];
    }
    return self;
}

-(void) customInit
{
    self.environmentTest = VSC::EnvironmentTest::SPtr(new VSC::EnvironmentTest);
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
}

-(void) windowWillLoad
{
    
}

-(void) awakeFromNib
{
    BOOST_ASSERT(self.sceneDetailScrollView);
    BOOST_ASSERT(self.sceneElementListView);
    BOOST_ASSERT(self.sceneDetailView);
    BOOST_ASSERT(self.tabBar);
    //BOOST_ASSERT(self.tabBox);
    
    self.sceneDetailScrollView.translatesAutoresizingMaskIntoConstraints = NO;
    self.sceneDetailView.translatesAutoresizingMaskIntoConstraints = NO;
    self.sceneElementListView.translatesAutoresizingMaskIntoConstraints = NO;
    self.tabBar.translatesAutoresizingMaskIntoConstraints = NO;
    
    [self setupConstraints];
    
    [self setupTabBar];
    
    self.sceneController.shootSpeed = 10.0;
    self.sceneController.cameraSpeed = 10.0;
    self.sceneController.cameraSensitivity = 0.13;
    
    self.tabBar.selectedIndex = 0;
}


-(BOOL)acceptsFirstResponder
{
    return NO;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)theEvent
{
    return NO;
}

#pragma mark - Custom Setters

- (void) setEnvironment:(VSC::Environment::WPtr)environment
{
    _environment = environment;
    [self reloadInterface];
    
}

#pragma mark - UI Setup

-(void) setupConstraints
{
    // setup constraints so that the width of the views are the same as that of their enclosing subviews
    NSLayoutConstraint* widthConstraint = [NSLayoutConstraint constraintWithItem:self.sceneDetailScrollView
                                                                       attribute:NSLayoutAttributeWidth
                                                                       relatedBy:NSLayoutRelationEqual
                                                                          toItem:self.sceneDetailView
                                                                       attribute:NSLayoutAttributeWidth
                                                                      multiplier:1.0
                                                                        constant:0.0];
    [self.sceneDetailScrollView addConstraint:widthConstraint];
}

-(void) setupTabBar
{
    
    BOOST_ASSERT(EnvironmentInspectorTabParamArray);
    
    NSMutableArray *items = [[NSMutableArray alloc] initWithCapacity:[EnvironmentInspectorTabParamArray count]];
    
    [EnvironmentInspectorTabParamArray enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        
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
            //[self.tabView selectTabViewItem:[tabView.tabViewItems objectAtIndex:tabBarItemIndex]];
            
            if ([tabBarItem.toolTip isEqualToString:VSCOSXTabTitleSceneElements])
            {
                NSLog(@"Selected scene element list tab");
                [self showSceneElementList];
            }
            else if ([tabBarItem.toolTip isEqualToString:VSCOSXTabTitleSceneSettings])
            {
                NSLog(@"Selected scene detail tab");
                [self showSceneDetail];
            }
            else if ([tabBarItem.toolTip isEqualToString:VSCOSXTabTitleEnveloppes])
            {
                NSLog(@"Selected enveloppes tab");
            }
            
        }
        else if (selectionType == DMTabBarItemSelectionType_DidSelect)
        {
            NSLog(@"%@ did select %lu/%@", self.tabBar, tabBarItemIndex, tabBarItem);
        }
        
    }];
    
}

#pragma mark - UI Helpers

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

-(void) resetInspectorView
{
    NSArray* subviews = [self.environmentInspectorView subviews];
    
    // remove all subviews except tabBar
    for (NSView* subview in subviews)
    {
        if (subview != self.tabBar)
        {
            [subview removeFromSuperview];
        }
    }
}

-(void) switchEnvironmentInspectorToTabView:(NSView*)tabView
{
    if (tabView == nil) return;
    
    BOOST_ASSERT(tabView);
    BOOST_ASSERT([tabView isKindOfClass:[NSView class]]);
    
    if ([tabView isKindOfClass:[NSView class]] == NO) return;
    if ([tabView superview] == self.environmentInspectorView) return;
    
    if (self.tabViewConstraints)
    {
        [self.environmentInspectorView removeConstraints:self.tabViewConstraints];
    }
    
    [self resetInspectorView];
    
    [self.environmentInspectorView  addSubview:tabView];

    NSView* bar = self.tabBar;
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(tabView, bar);
    
    NSMutableArray* allConstraints = [NSMutableArray array];
    
    NSArray* hConstraints =[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[tabView]|"
                                                                   options:0
                                                                   metrics:nil
                                                                     views:viewsDictionary];
    
    [allConstraints addObjectsFromArray:hConstraints];
    [self.environmentInspectorView addConstraints:hConstraints];
    
    NSArray* vConstraints =[NSLayoutConstraint constraintsWithVisualFormat:@"V:[bar]-0-[tabView]|"
                                                                   options:0
                                                                   metrics:nil
                                                                     views:viewsDictionary];
    
    [allConstraints addObjectsFromArray:vConstraints];
    [self.environmentInspectorView addConstraints:vConstraints];
    
    self.tabViewConstraints = [NSArray arrayWithArray:allConstraints];
}


-(void) showSceneElementList
{
    BOOST_ASSERT(self.sceneElementListView);
    BOOST_ASSERT(self.sceneElementListView.elementTableView);
    BOOST_ASSERT(self.sceneElementListView.elementTableView.delegate == self.sceneController);
    
    
    if ([self.sceneElementListView superview] == self.environmentInspectorView) return; // we are already showing element list
    
    [self switchEnvironmentInspectorToTabView:self.sceneElementListView];
    
    [self.sceneElementListView.elementTableView reloadData];
    
}

-(void) showSceneDetail
{
    BOOST_ASSERT(self.sceneDetailScrollView);
    BOOST_ASSERT(self.sceneDetailView);
    
    if ([self.sceneDetailView superview] == self.environmentInspectorView) return; // we are already showing element list
    
    [self resetInspectorView];
    
    [self switchEnvironmentInspectorToTabView:self.sceneDetailScrollView];
    
    [self.sceneDetailView reloadInterface];
    
}


-(void) showElementInspectorForElement:(VSC::OB::Element::SPtr)element
{
    NSLog(@"%@ showElementInspectorForElement", self);
    
    if (!self.elementInspectorWindowController)
    {
        NSString* nibName = @"VSCOSXOBSceneElementInspectorWindowController";
        self.elementInspectorWindowController = [[VSCOSXOBSceneElementInspectorWindowController alloc] initWithWindowNibName:nibName];
        BOOST_ASSERT(self.elementInspectorWindowController);
        BOOST_ASSERT(self.elementInspectorWindowController.elementInspectorViewController);
        self.elementInspectorWindowController.elementInspectorViewController.environmentController = self;
    }
    
    BOOST_ASSERT(self.elementInspectorWindowController.elementInspectorViewController);
    self.elementInspectorWindowController.elementInspectorViewController.element = VSC::OB::Element::WPtr(element);
    [self.elementInspectorWindowController showWindow:self];
    [self.elementInspectorWindowController.elementInspectorViewController showElementDetailView];
}

-(VSC::IM::CollisionEventChain::SPtr) collisionStartedEventChainForElement:(VSC::OB::Element::SPtr)element
{
    VSC::Environment::SPtr env = self.environment.lock();
    BOOST_ASSERT(env);
    
    if (env)
    {
        BOOST_ASSERT(env->getIMCollisionMapper());
        if (env->getIMCollisionMapper()) return env->getIMCollisionMapper()->getEventChainForCollisionStarted(element);
    }
    
    return VSC::IM::CollisionEventChain::SPtr();
}

-(VSC::IM::CollisionEventChain::SPtr) collisionEndedEventChainForElement:(VSC::OB::Element::SPtr)element
{
    VSC::Environment::SPtr env = self.environment.lock();
    BOOST_ASSERT(env);
    
    if (env)
    {
        return env->getIMCollisionMapper()->getEventChainForCollisionEnded(element);
    }
    
    return VSC::IM::CollisionEventChain::SPtr();
}

#pragma mark - Sensible tests

-(IBAction)resetAction:(id)sender
{
    [self setupTest];
}

-(void)setupTest
{
    BOOST_ASSERT(self.environmentTest);
    VSC::Environment::SPtr env = self.environment.lock();
    BOOST_ASSERT(env);
    
    if (self.environmentTest && env)
    {
        self.environmentTest->setupTestForEnvironment(env);
    }
}

#pragma mark - Stupid tests

- (IBAction)test1:(id)sender {
    
}

- (IBAction)test2:(id)sender {

}

- (IBAction)test3:(id)sender {
    
}

@end
