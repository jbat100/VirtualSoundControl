//
//  VSCOBOSXSceneControlView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXEnvironmentInspectorView.h"

#import "PXListView.h"
#import "VSCOSXEnvironmentController.h"
#import "DMTabBar.h"
#import "VSCOSXOBSceneElementListView.h"
#import "VSCOSXOBSceneDetailView.h"
#import "VSCOSXOBSceneController.h"


NSArray* EnvironmentInspectorTabParamArray = nil;

NSString* const VSCOSXTabTitleSceneSettings = @"Scene Settings";
NSString* const VSCOSXTabTitleSceneElements = @"Scene Elements";
NSString* const VSCOSXTabTitleEnveloppes = @"Enveloppes";

@interface VSCOSXEnvironmentInspectorView ()

/*
 *  Tabbed interface
 */

@property (weak) IBOutlet DMTabBar* tabBar;
//@property (nonatomic, strong) IBOutlet NSTabView* tabView;

@property (weak) IBOutlet NSView* mainView;

-(void) setup;

-(void) clearMainView;

@end

@implementation VSCOSXEnvironmentInspectorView

+(void) load {
    
    EnvironmentInspectorTabParamArray = @[
    @{@"image": [NSImage imageNamed:@"158-wrench-2.png"], @"title": VSCOSXTabTitleSceneSettings},
    @{@"image": [NSImage imageNamed:@"12-eye"], @"title": VSCOSXTabTitleSceneElements},
    @{@"image": [NSImage imageNamed:@"122-stats"], @"title": VSCOSXTabTitleEnveloppes}
    ];
    
}

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
        [self setup];
    }
    
    return self;
}

-(void) dealoc
{
    NSLog(@"%@ DEALLOC", self);
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void) awakeFromNib
{
    [self setup];
    
    BOOST_ASSERT(self.environmentController);
    BOOST_ASSERT(self.sceneElementListView);
    BOOST_ASSERT(self.tabBar);
    BOOST_ASSERT(self.mainView);
    
    NSLog(@"%@ awakeFromNib, self.sceneElementListView: %@, subviews: %@",
          self, self.sceneElementListView, [self.sceneElementListView subviews]);
}

-(void) setup
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

-(void) clearMainView
{
    NSArray* subviews = [self.mainView subviews];
    
    for (NSView* subview in subviews)
    {
        [subview removeFromSuperview];
    }
}

-(void) showSceneElementList
{
    BOOST_ASSERT(self.sceneElementListView);
    BOOST_ASSERT(self.sceneElementListView.listView);
    BOOST_ASSERT(self.sceneElementListView.listView.delegate);
    
    NSLog(@"%@ showSceneElementList", self);
    
    //[self clearMainView];
    
    [self.sceneDetailView setHidden:YES];
    
    if ([self.sceneElementListView superview] != self.mainView)
    {
        [self.mainView addSubview:self.sceneElementListView];
    }
    
    [self.sceneElementListView setHidden:NO];
    
    self.sceneElementListView.frame = self.mainView.bounds;
    
    [self.sceneElementListView.listView reloadData];
    
    NSLog(@"self.sceneElementListView.frame: %@", NSStringFromRect(self.sceneElementListView.frame));
    NSLog(@"self.sceneElementListView.listView.frame: %@", NSStringFromRect(self.sceneElementListView.listView.frame));
}

-(void) showSceneDetail
{
    NSLog(@"%@ showSceneDetail", self);
    
    if (!self.sceneDetailView)
    {
        BOOST_ASSERT(self.environmentController.sceneController);
        
        if (self.environmentController.sceneController)
        {
            NSArray* topLevelObjects = nil;
            
            [[NSBundle mainBundle] loadNibNamed:@"VSCOSXOBSceneDetailView"
                                          owner:self.environmentController.sceneController
                                topLevelObjects:&topLevelObjects];
            
            for (NSObject* topLevelObject in topLevelObjects)
            {
                if ([topLevelObject isKindOfClass:[VSCOSXOBSceneDetailView class]])
                {
                    self.sceneDetailView = (VSCOSXOBSceneDetailView*)topLevelObject;
                }
            }
            
            BOOST_ASSERT(self.sceneDetailView);
            BOOST_ASSERT(self.sceneDetailView.sceneController == self.environmentController.sceneController);
        }
        
    }
    
    //[self clearMainView];
    
    [self.sceneElementListView setHidden:YES];
    
    if ([self.sceneDetailView superview] != self.mainView)
    {
        [self.mainView addSubview:self.sceneDetailView];
    }
    
    [self.sceneDetailView setHidden:NO];
    
    self.sceneDetailView.frame = self.mainView.bounds;
    
    [self.sceneDetailView reloadInterface];
    
}

@end
