//
//  VSCOSXOBSceneElementInspectorController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "VSCOSXEnvironmentController.h"
#import "VSCOSXOBSceneElementEditor.h"

#include "VSCOBScene.h"
#include "VSCIMEvent.h"

@protocol VSCIMOSXCollisionEventChainController;

/*
 *  Main controller for a Scene::Element
 */

@protocol VSCOSXOBSceneElementController <NSObject>

@property (nonatomic, assign) VSC::OB::Scene::Element::WPtr element;

@property (weak) id <VSCOSXEnvironmentController> environmentController;

-(void) showElementDetailView;
-(void) showElementCollisionView;

@end


