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

const static BOOL traceInterface = YES;

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
    BOOST_ASSERT(self.view);
    self.view.translatesAutoresizingMaskIntoConstraints = NO;
    
    BOOST_ASSERT(self.tabBar);
    [self setupTabBar];
    
    BOOST_ASSERT(self.mainBox.contentView);
    BOOST_ASSERT(self.elementDetailView);
    BOOST_ASSERT(self.elementDetailView.elementController == self);
    
    [self.elementDetailView reloadWholeInterface];
    
    BOOST_ASSERT(self.elementCollisionView);
    BOOST_ASSERT(self.elementCollisionView.elementController == self);
    
    BOOST_ASSERT(self.elementCollisionView.collisionStartedEventChainBox);
    if (self.elementCollisionView.collisionStartedEventChainBox)
    {
        NSView* boxContentView = self.elementCollisionView.collisionStartedEventChainBox.contentView;
        NSView* eventChainView = self.collisionStartedEventChainViewController.view;
        
        BOOST_ASSERT(boxContentView);
        BOOST_ASSERT(eventChainView);
        
        [boxContentView addSubview:eventChainView];
        
        NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(eventChainView);
        [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[eventChainView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
        [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[eventChainView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
        
        [self.collisionStartedEventChainViewController reloadInterface];
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
    
    if (traceInterface) NSLog(@"%@ setElement %p", self, newElement.get());
    
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
        BOOST_ASSERT(self.environmentController);
        
        collisionStartedEventChain = [self.environmentController collisionStartedEventChainForElement:elem];
        collisionEndedEventChain = [self.environmentController collisionEndedEventChainForElement:elem];
    }
    
    self.collisionStartedEventChainViewController.eventChain = collisionStartedEventChain;
    self.collisionEndedEventChainViewController.eventChain = collisionEndedEventChain;
    
}

#pragma mark - UI Callbacks

-(IBAction) immobilized:(id)sender
{
    BOOST_ASSERT(sender);
    BOOST_ASSERT(sender == self.elementDetailView.immobilizedCheckBox);
    
    if (sender && sender == self.elementDetailView.immobilizedCheckBox)
    {
        VSC::OB::Scene::Element::SPtr elem = self.element.lock();
        BOOST_ASSERT(elem);
        if (elem)
        {
            if ([self.elementDetailView.immobilizedCheckBox state] == NSOnState)
            {
                elem->setImmobilized(true);
            }
            else
            {
                elem->setImmobilized(false);
            }
        }
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


-(void) showElementDetailView
{
    if (traceInterface) NSLog(@"%@ showElementDetailView", self);
    
    BOOST_ASSERT(self.elementDetailView);
    BOOST_ASSERT(self.mainBox);
    
    self.tabBar.selectedIndex = 0;
    NSView* boxContentView = self.mainBox.contentView;
    
    if ([self.elementDetailView superview] == boxContentView) return; // we are already showing the element detail view...
    
    [[boxContentView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    // remove all old constraints
    [boxContentView removeConstraints:[boxContentView constraints]];
    [boxContentView addSubview:self.elementDetailView];
    
    {
        NSView* detailView = self.elementDetailView;
        NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(detailView);
        [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[detailView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
        [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[detailView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
    }
    
}

-(void) showElementCollisionView
{
    if (traceInterface) NSLog(@"%@ showElementCollisionView", self);
    
    BOOST_ASSERT(self.elementCollisionView);
    BOOST_ASSERT(self.mainBox);
    
    self.tabBar.selectedIndex = 0;
    NSView* boxContentView = self.mainBox.contentView;
    
    if ([self.elementCollisionView superview] == boxContentView) return; // we are already showing the element detail view...
    
    [[boxContentView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    // remove all old constraints
    [boxContentView removeConstraints:[boxContentView constraints]];
    [boxContentView addSubview:self.elementCollisionView];
    
    {
        NSView* collisionView = self.elementCollisionView;
        NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(collisionView);
        [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[collisionView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
        [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[collisionView]|"
                                                                               options:0 metrics:nil views:viewsDictionary]];
    }
    
    [self updateCollisionEventChains];
    
}

#pragma mark - VSCOBOSXSceneListenerTarget

-(void) sceneWasRendered:(VSC::OB::Scene::SPtr)scene
{
    [self.elementDetailView reloadPositionInterface:NO];
}


@end
