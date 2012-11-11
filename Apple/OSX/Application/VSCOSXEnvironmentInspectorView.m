//
//  VSCOBOSXSceneControlView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXEnvironmentInspectorView.h"

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

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void) awakeFromNib
{
    [self setup];
}

-(void) setup
{
    
    BOOST_ASSERT(EnvironmentInspectorTabParamArray);
    
    NSMutableArray *items = [[NSMutableArray alloc] initWithCapacity:[EnvironmentInspectorTabParamArray count]];
    
    [EnvironmentInspectorTabParamArray enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        
        NSImage *iconImage = [objDict objectForKey:@"image"];
        [iconImage setTemplate:YES];
        
        DMTabBarItem *item = [DMTabBarItem tabBarItemWithIcon:iconImage tag:idx];
        item.toolTip = [objDict objectForKey:@"title"];
        item.keyEquivalent = [NSString stringWithFormat:@"%d",idx];
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
            
            if ([item.toolTip isEqualToString:VSCOSXTabTitleScene])
            {
                [self showElementSceneList];
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
    
}

@end
