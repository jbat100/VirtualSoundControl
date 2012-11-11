//
//  VSCOBOSXSceneControlView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXEnvironmentInspectorView.h"

#import "VSCOSXEnvironmentController.h"
#import "DMTabBar.h"
#import "VSCOSXOBSceneElementListView.h"
#import "VSCOSXOBSceneElementListController.h"


NSArray* EnvironmentInspectorTabParamArray = nil;

NSString* const VSCOSXTabTitleScene = @"Scene";
NSString* const VSCOSXTabTitleEnveloppes = @"Enveloppes";

@interface VSCOSXEnvironmentInspectorView ()

-(void) setup;

@end

@implementation VSCOSXEnvironmentInspectorView

+(void) load {
    
    EnvironmentInspectorTabParamArray = @[
    @{@"image": [NSImage imageNamed:@"12-eye"], @"title": VSCOSXTabTitleScene},
    @{@"image": [NSImage imageNamed:@"122-stats"], @"title": VSCOSXTabTitleEnveloppes},
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
            
            if ([tabBarItem.toolTip isEqualToString:VSCOSXTabTitleScene])
            {
                [self showSceneElementList];
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
    BOOST_ASSERT(self.sceneElementListView);
    BOOST_ASSERT(self.sceneElementListView.listView);
    BOOST_ASSERT(self.sceneElementListView.listView.delegate);
    
    if ([self.sceneElementListView superview] != self)
    {
        [self addSubview:self.sceneElementListView];
    }
    
    self.sceneElementListView.frame = self.bounds;
    
    [self.sceneElementListView.listView reloadData];
}

@end
