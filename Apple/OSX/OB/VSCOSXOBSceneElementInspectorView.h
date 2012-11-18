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

@property (strong) IBOutlet VSCOSXOBSceneElementDetailView* elementDetailView;
@property (strong) IBOutlet VSCOSXOBSceneElementCollisionView* elementCollisionView;

-(void) showElementDetailView;
-(void) showElementCollisionView;

@end
