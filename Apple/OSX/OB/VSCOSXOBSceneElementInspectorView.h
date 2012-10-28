//
//  VSCOSXOBSceneElementInspectorView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "DMTabBar.h"

@interface VSCOSXOBSceneElementInspectorView : NSView

@property (nonatomic, strong) IBOutlet DMTabBar* tabBar;

@property (nonatomic, strong) IBOutlet NSTabView* tabView;

@end
