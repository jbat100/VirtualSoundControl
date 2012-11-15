//
//  VSCOBSceneElementListController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCOSXEnvironmentController.h"
#import "VSCOBOSXSceneListener.h"

#include "VSCOBScene.h"

@class VSCOSXOBSceneElementListView;

@protocol VSCOSXOBSceneController <NSObject, PXListViewDelegate, VSCOBOSXSceneListenerTarget>

@property (nonatomic, assign) VSC::OB::Scene::WPtr scene;

@property (assign) id<VSCOSXEnvironmentController> environmentController;

@end

@interface VSCOSXOBSceneController : NSObject <VSCOSXOBSceneController>

@property (weak) VSCOSXOBSceneElementListView* elementListView;

@end
