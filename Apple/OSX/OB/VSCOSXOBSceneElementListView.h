//
//  VSCOSXOBSceneElementListView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

//#include "VSCOBScene.h"

@class PXListView;


@interface VSCOSXOBSceneElementListView : NSView

@property (weak) IBOutlet PXListView* listView;

@end
