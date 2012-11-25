//
//  VSCOSXOBSceneElementInspectorView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementInspectorView.h"

#include <boost/assert.hpp>

NSString* const VSCOSXTabTitleElementDetails = @"Details";
NSString* const VSCOSXTabTitleElementCollision = @"Collision";

NSArray* ElementInspectorTabParamArray = nil;

@interface VSCOSXOBSceneElementInspectorView ()

@property (weak) IBOutlet DMTabBar* tabBar;
@property (weak) IBOutlet NSView* mainView;

-(void) commonInit;

-(void) setupTabBar;

@end

@implementation VSCOSXOBSceneElementInspectorView

+ (void)load
{
    ElementInspectorTabParamArray = @[
    @{@"image": [NSImage imageNamed:@"19-gear.png"], @"title": VSCOSXTabTitleElementDetails},
    @{@"image": [NSImage imageNamed:@"55-network.png"], @"title": VSCOSXTabTitleElementCollision},
    ];
}

#pragma mark - NSView

- (void)awakeFromNib
{
    NSLog(@"%@ awakeFromNib", self);
    
    //BOOST_ASSERT(self.elementDetailView);
    //BOOST_ASSERT(self.elementCollisionView);
    
    [self setupTabBar];
    
    
    /*
    [self showElementDetailView];
    self.tabBar.selectedIndex = 0;
    */
}

-(id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
        [self commonInit];
    }
    return self;
}

-(id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        // Initialization code here.
        [self commonInit];
    }
    return self;
}

-(void) commonInit
{
    self.translatesAutoresizingMaskIntoConstraints = NO;
}


- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void) setElementController:(id<VSCOSXOBSceneElementController>)elementController
{
    _elementController = elementController;
    
    self.elementDetailView.elementController = elementController;
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
            //[self.tabView selectTabViewItem:[tabView.tabViewItems objectAtIndex:tabBarItemIndex]];
            
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
    if (!self.elementDetailView)
    {
        NSArray* topLevelObjects = nil;
        
        [[NSBundle mainBundle] loadNibNamed:@"VSCOSXOBSceneElementDetailView"
                                      owner:self
                            topLevelObjects:&topLevelObjects];
        
        BOOST_ASSERT(self.elementDetailView);
    }
    
    self.elementDetailView.elementController = self.elementController;
    self.tabBar.selectedIndex = 0;
    [self.elementCollisionView setHidden:YES];
    
    if (!self.elementDetailView) return;
    
    if ([self.elementDetailView superview] != self.mainView)
    {
        [self.mainView addSubview:self.elementDetailView];
    }
    self.elementDetailView.frame = self.mainView.bounds;
    [self.elementDetailView setHidden:NO];
    
    [self.elementDetailView reloadWholeInterface];
    
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(_elementDetailView);
    [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_elementDetailView(>=200)]|"
                                                                 options:0
                                                                 metrics:nil
                                                                   views:viewsDictionary]];
    [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[_elementDetailView(>=350)]|"
                                                                 options:0
                                                                 metrics:nil
                                                                   views:viewsDictionary]];
}

-(void) showElementCollisionView
{
    if (!self.elementCollisionView)
    {
        NSArray* topLevelObjects = nil;
        
        [[NSBundle mainBundle] loadNibNamed:@"VSCOSXOBSceneElementCollisionView"
                                      owner:self
                            topLevelObjects:&topLevelObjects];
        
        //BOOST_ASSERT(self.elementCollisionView);
    }
    
    self.tabBar.selectedIndex = 1;
    [self.elementDetailView setHidden:YES];
    
    if(!self.elementCollisionView) return;
    
    if ([self.elementCollisionView superview] != self.mainView)
    {
        [self.mainView addSubview:self.elementCollisionView];
    }
    self.elementCollisionView.frame = self.mainView.bounds;
    [self.elementCollisionView setHidden:NO];
    
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(_elementCollisionView);
    [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_elementCollisionView(>=200)]|"
                                                                 options:0
                                                                 metrics:nil
                                                                   views:viewsDictionary]];
    [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[_elementCollisionView(>=350)]|"
                                                                 options:0
                                                                 metrics:nil
                                                                   views:viewsDictionary]];
}

@end
