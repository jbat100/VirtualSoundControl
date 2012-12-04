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
@property (weak) IBOutlet NSBox* tabBox;

-(void) reloadSceneInterface;
-(void) setupTabBar;

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
}

-(void) windowWillLoad
{
    
}

-(void) awakeFromNib
{
    
    BOOST_ASSERT(self.sceneElementListView);
    BOOST_ASSERT(self.sceneDetailView);
    BOOST_ASSERT(self.tabBar);
    BOOST_ASSERT(self.tabBox);
    
    [self setupTabBar];
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


-(void) showSceneElementList
{
    BOOST_ASSERT(self.tabBox);
    BOOST_ASSERT(self.sceneElementListView);
    BOOST_ASSERT(self.sceneElementListView.elementTableView);
    BOOST_ASSERT(self.sceneElementListView.elementTableView.delegate == self.sceneController);
    
    NSView* boxContentView = [self.tabBox contentView];
    
    if ([self.sceneElementListView superview] == boxContentView) return; // we are already showing element list
    
    [[boxContentView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    // remove all old constraints
    [boxContentView removeConstraints:[boxContentView constraints]];
    [boxContentView addSubview:self.sceneElementListView];
    
    {
        NSView* elementListView  = self.sceneElementListView;
        NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(elementListView);
        [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[elementListView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
        [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[elementListView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
    }
    
    [self.sceneElementListView.elementTableView reloadData];
    
}

-(void) showSceneDetail
{
    BOOST_ASSERT(self.tabBox);
    BOOST_ASSERT(self.sceneDetailView);
    
    NSView* boxContentView = [self.tabBox contentView];
    
    if ([self.sceneDetailView superview] == boxContentView) return; // we are already showing element list
    
    [[boxContentView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    // remove all old constraints
    [boxContentView removeConstraints:[boxContentView constraints]];
    [boxContentView addSubview:self.sceneDetailView];
    
    {
        NSView* sceneDetailView  = self.sceneDetailView;
        NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(sceneDetailView);
        [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[sceneDetailView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
        [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[sceneDetailView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
    }
    
    [self.sceneDetailView reloadInterface];
    
}


-(void) showElementInspectorForElement:(VSC::OB::Scene::Element::SPtr)element
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
        BOOST_ASSERT(env->getIMCollisionMapper());
        if (env->getIMCollisionMapper()) return env->getIMCollisionMapper()->getEventChainForCollisionStarted(element);
    }
    
    return VSC::IM::CollisionEventChain::SPtr();
}

-(VSC::IM::CollisionEventChain::SPtr) collisionEndedEventChainForElement:(VSC::OB::Scene::Element::SPtr)element
{
    VSC::Environment::SPtr env = self.environment.lock();
    BOOST_ASSERT(env);
    
    if (env)
    {
        return env->getIMCollisionMapper()->getEventChainForCollisionEnded(element);
    }
    
    return VSC::IM::CollisionEventChain::SPtr();
}

#pragma mark - Stupid tests

- (IBAction)test1:(id)sender {
    
}

- (IBAction)test2:(id)sender {

}

- (IBAction)test3:(id)sender {
    
}

@end
