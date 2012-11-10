//
//  VSCOSXOBSceneElementInspectorView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "DMTabBar.h"
#import "VSCOSXOBSceneElementDetailView.h"
#import "VSCOSXOBSceneElementCollisionView.h"

@interface VSCOSXOBSceneElementInspectorView : NSView

@property (weak) IBOutlet DMTabBar* tabBar;
@property (weak) IBOutlet NSView* mainView;

@property (strong) VSCOSXOBSceneElementDetailView* elementDetailView;
@property (strong) VSCOSXOBSceneElementCollisionView* elementCollisionView;

-(void) showElementDetailView;
-(void) showElementCollisionView;

@end
