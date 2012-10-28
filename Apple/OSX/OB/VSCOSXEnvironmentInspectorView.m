//
//  VSCOBOSXSceneControlView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXEnvironmentInspectorView.h"

NSArray* EnvironmentInspectorTabParamArray = nil;

@interface VSCOSXEnvironmentInspectorView ()


@end

@implementation VSCOSXEnvironmentInspectorView

+(void) load {
    
    EnvironmentInspectorTabParamArray = @[
    @{@"image": [NSImage imageNamed:@"12-eye"], @"title": @"Scene"},
    @{@"image": [NSImage imageNamed:@"122-stats"], @"title": @"Enveloppes"},
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

-(void) setup
{
    
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
            NSLog(@"Will select %lu/%@", tabBarItemIndex, tabBarItem);
            [self.tabView selectTabViewItem:[tabView.tabViewItems objectAtIndex:tabBarItemIndex]];
        }
        else if (selectionType == DMTabBarItemSelectionType_DidSelect)
        {
            NSLog(@"Did select %lu/%@", tabBarItemIndex, tabBarItem);
        }
        
    }];
    
}

@end
