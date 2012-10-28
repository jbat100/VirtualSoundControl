//
//  VSCOBSceneElementListController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "PXListView.h"
#import "VSCOSXEnvironmentController.h"

#include "VSCOBScene.h"

@interface VSCOSXOBSceneElementListController : NSObject <PXListViewDelegate>

@property (weak) id<VSCOSXEnvironmentController> environmentController;

@property (nonatomic, assign) VSC::OB::Scene::WPtr scene;

@property (nonatomic, strong) IBOutlet PXListView* sceneElementListView;

@end
