//
//  VSCOBOSXSceneElementListView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCOBScene.h"

@class PXListView;


@interface VSCOBOSXSceneElementListView : NSView

@property (nonatomic, strong) IBOutlet PXListView* listView;

@end
