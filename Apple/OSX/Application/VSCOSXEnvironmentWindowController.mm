//
//  VSCOBOSXSceneWindow.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXEnvironmentWindowController.h"
#import "VSCOSXApplicationManager.h"
#import "VSCOSXEnvironmentController.h"

#import "VSCOBOSXElementInspectorWindowController.h"
#import "VSCOBOSXElementInspectorViewController.h"
#import "VSCIMOSXEventChainWindowController.h"
#import "VSCIMOSXEventChainViewController.h"

#import "VSCOBOSXSceneController.h"
#import "VSCOBOSXSceneDisplayView.h"
#import "VSCOBOSXElementListView.h"
#import "VSCOBOSXSceneDetailView.h"

#import "VSCIMOSXEventChainListView.h"
#import "VSCIMOSXEventChainCellView.h"

#import "NSString+VSCAdditions.h"
#import "NSArray+VSCAdditions.h"

#import "DMTabBar.h"

#include "VSCEnvironment.h"
#include "VSCOB.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCIM.h"
#include "VSCCollisionMapper.h"

#include <Ogre/Ogre.h>

#include <vector>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

NSArray* EnvironmentInspectorTabParamArray = nil;

NSString* const VSCOSXTabTitleSceneSettings = @"Scene Settings";
NSString* const VSCOSXTabTitleElements = @"Scene Elements";
NSString* const VSCOSXTabEventChains = @"Event Chains";
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
    @{@"image": [NSImage imageNamed:@"12-eye"], @"title": VSCOSXTabTitleElements},
    //@{@"image": [NSImage imageNamed:@"122-stats"], @"title": VSCOSXTabTitleEnveloppes}
    @{@"image": [NSImage imageNamed:@"122-stats"], @"title": VSCOSXTabEventChains}
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
    self.environmentTest = EnvironmentTest::SPtr(new EnvironmentTest);
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
    BOOST_ASSERT(self.elementListView);
    BOOST_ASSERT(self.sceneDetailView);
    BOOST_ASSERT(self.tabBar);
    //BOOST_ASSERT(self.tabBox);
    
    self.sceneDetailScrollView.translatesAutoresizingMaskIntoConstraints = NO;
    self.sceneDetailView.translatesAutoresizingMaskIntoConstraints = NO;
    self.elementListView.translatesAutoresizingMaskIntoConstraints = NO;
    self.eventChainListView.translatesAutoresizingMaskIntoConstraints = NO;
    self.tabBar.translatesAutoresizingMaskIntoConstraints = NO;
    
    [self setupConstraints];
    
    [self setupTabBar];
    
    self.sceneController.shootSpeed = 10.0;
    self.sceneController.shootSize = 0.2;
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

- (void) setEnvironment:(Environment::WPtr)environment
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
            
            if ([tabBarItem.toolTip isEqualToString:VSCOSXTabTitleElements])
            {
                NSLog(@"Selected scene element list tab");
                [self showElementList];
            }
            else if ([tabBarItem.toolTip isEqualToString:VSCOSXTabTitleSceneSettings])
            {
                NSLog(@"Selected scene detail tab");
                [self showSceneDetail];
            }
            else if ([tabBarItem.toolTip isEqualToString:VSCOSXTabEventChains])
            {
                NSLog(@"Selected event chains");
                [self showEventChainList];
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
    Environment::SPtr env = self.environment.lock();
    [self reloadSceneInterface];
}

-(void) reloadSceneInterface
{
    Environment::SPtr env = self.environment.lock();
    
    if (env)
    {
        self.sceneController.scene = Scene::WPtr(env->getScene());
    }
    else
    {
        self.sceneController.scene = Scene::WPtr();
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
    
    NSArray* vConstraints =[NSLayoutConstraint constraintsWithVisualFormat:@"V:[bar(==25)]-0-[tabView]|"
                                                                   options:0
                                                                   metrics:nil
                                                                     views:viewsDictionary];
    
    [allConstraints addObjectsFromArray:vConstraints];
    [self.environmentInspectorView addConstraints:vConstraints];
    
    self.tabViewConstraints = [NSArray arrayWithArray:allConstraints];
}


-(void) showElementList
{
    BOOST_ASSERT(self.elementListView);
    BOOST_ASSERT(self.elementListView.elementTableView);
    BOOST_ASSERT(self.elementListView.elementTableView.delegate == self.sceneController);
    
    
    if ([self.elementListView superview] != self.environmentInspectorView)
    {
        [self switchEnvironmentInspectorToTabView:self.elementListView];
    }
    
    [self.elementListView.elementTableView reloadData];
    
}

-(void) showEventChainList
{
    BOOST_ASSERT(self.eventChainListView);
    BOOST_ASSERT(self.eventChainListView.tableView);
    BOOST_ASSERT(self.eventChainListView.tableView.delegate == self);
    
    if ([self.eventChainListView superview] != self.environmentInspectorView)
    {
        [self switchEnvironmentInspectorToTabView:self.eventChainListView];
    }
    
    [self.eventChainListView.tableView reloadData];
}

-(void) showSceneDetail
{
    BOOST_ASSERT(self.sceneDetailScrollView);
    BOOST_ASSERT(self.sceneDetailView);
    
    if ([self.sceneDetailView superview] != self.environmentInspectorView)
    {
        [self resetInspectorView];
        [self switchEnvironmentInspectorToTabView:self.sceneDetailScrollView];
    }
}

-(void) showElementInspectorForElement:(Element::SPtr)element
{
    NSLog(@"%@ showElementInspectorForElement", self);
    
    if (!self.elementInspectorWindowController)
    {
        NSString* nibName = @"VSCOBOSXElementInspectorWindowController";
        self.elementInspectorWindowController = [[VSCOBOSXElementInspectorWindowController alloc] initWithWindowNibName:nibName];
        BOOST_ASSERT(self.elementInspectorWindowController);
        BOOST_ASSERT(self.elementInspectorWindowController.elementInspectorViewController);
        self.elementInspectorWindowController.elementInspectorViewController.environmentController = self;
    }
    
    BOOST_ASSERT(self.elementInspectorWindowController.elementInspectorViewController);
    self.elementInspectorWindowController.elementInspectorViewController.element = Element::WPtr(element);
    [self.elementInspectorWindowController showWindow:self];
    [self.elementInspectorWindowController.elementInspectorViewController showElementDetailView];
}

-(void) showEventChainEditor:(EventChain::SPtr)eventChain
{
    NSLog(@"%@ showEventChainEditor", self);
    
    if (!self.eventChainWindowController)
    {
        NSString* nibName = @"VSCIMOSXEventChainWindowController";
        self.eventChainWindowController = [[VSCIMOSXEventChainWindowController alloc] initWithWindowNibName:nibName];
        BOOST_ASSERT(self.eventChainWindowController);
        BOOST_ASSERT(self.eventChainWindowController.eventChainViewController);
        self.eventChainWindowController.eventChainViewController.environmentController = self;
    }
    
    BOOST_ASSERT(self.eventChainWindowController.eventChainViewController);
    self.eventChainWindowController.eventChainViewController.eventChain = EventChain::WPtr(eventChain);
    [self.eventChainWindowController showWindow:self];
    
}


#pragma mark - NSTableView Delagate / DataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    BOOST_ASSERT(aTableView == self.eventChainListView.tableView);
    
    if (aTableView == self.eventChainListView.tableView)
    {
        Environment::SPtr env = self.environment.lock();
        BOOST_ASSERT(env);
        if (env)
        {
            const EventChains& eventChains = env->getEventChains();
            return eventChains.size();
        }
    }
    
	return 0;
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row
{
    BOOST_ASSERT(tableView == self.eventChainListView.tableView);
    
    if (tableView == self.eventChainListView.tableView)
    {
        Environment::SPtr env = self.environment.lock();
        BOOST_ASSERT(env);
        if (env)
        {
            const EventChains& eventChains = env->getEventChains();
            BOOST_ASSERT(eventChains.size() > row);
            if (eventChains.size() > row)
            {
                VSCIMOSXEventChainCellView* cell = [tableView makeViewWithIdentifier:@"VSCIMOSXEventChainCellView" owner:self];
                BOOST_ASSERT(cell);
                BOOST_ASSERT([cell isKindOfClass:[VSCIMOSXEventChainCellView class]]);
                
                EventChain::SPtr eventChain = eventChains.at(row);
                BOOST_ASSERT(eventChain);
                if (eventChain)
                {
                    [cell setEventChain:EventChain::WPtr(eventChain)];
                    return cell;
                }
            }
            
        }
        
    }
    
	return nil;
}

- (CGFloat)tableView:(NSTableView *)tableView heightOfRow:(NSInteger)row
{
    
    BOOST_ASSERT(tableView == self.eventChainListView.tableView);
    
    if (tableView == self.eventChainListView.tableView)
    {
        return [VSCIMOSXEventChainCellView defaultViewHeight];
    }
    
    return 0.0;
}

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
    NSLog(@"%@ tableViewSelectionDidChange: %@", self, aNotification);
    
    /*
    
    BOOST_ASSERT([aNotification object] == self.eventChainListView.tableView);
    if ([aNotification object] != self.eventChainListView.tableView) return;
    
    NSIndexSet* rowIndexes = [self.eventChainListView.tableView selectedRowIndexes];
    
    [rowIndexes enumerateIndexesUsingBlock:^(NSUInteger idx, BOOL *stop) {
        NSView* v = [self.eventChainListView.tableView viewAtColumn:0 row:idx makeIfNecessary:NO];
        if ([v isKindOfClass:[VSCIMOSXEventChainCellView class]])
        {
            VSCIMOSXEventChainCellView* eventChainView = (VSCIMOSXEventChainCellView*)v;
            //[actionView setNeedsLayout:YES];
            [eventChainView printUIDescription];
        }
    }];
     
     */

}

- (void)tableViewColumnDidResize:(NSNotification *)aNotification
{
    NSLog(@"%@ tableViewColumnDidResize: %@, column %@, old width %@",
          self, [aNotification object],
          [[aNotification userInfo] objectForKey:@"NSTableColumn"],
          [[aNotification userInfo] objectForKey:@"NSOldWidth"]);
}

- (void)tableView:(NSTableView *)aTableView
  willDisplayCell:(id)aCell
   forTableColumn:(NSTableColumn *)aTableColumn
              row:(NSInteger)rowIndex
{
    
}


#pragma mark - Sensible tests

-(IBAction)resetAction:(id)sender
{
    [self setupTest];
}

-(void)setupTest
{
    BOOST_ASSERT(self.environmentTest);
    Environment::SPtr env = self.environment.lock();
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
