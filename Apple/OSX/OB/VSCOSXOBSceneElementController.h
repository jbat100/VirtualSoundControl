//
//  VSCOSXOBSceneElementInspectorController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCOBScene.h"

@protocol VSCOSXEnvironmentController;
@protocol VSCIMOSXCollisionEventChainController;
//@class VSCOSXOBSceneElementInspectorView;

/*
 *  Main controller for a Scene::Element
 */

@protocol VSCOSXOBSceneElementController <NSObject>

@property (nonatomic, assign) VSC::OB::Scene::Element::WPtr element;

@property (weak) id <VSCOSXEnvironmentController> environmentController;
@property (strong) id<VSCIMOSXCollisionEventChainController> collisionEventChainController;

-(void) showElementDetailView;
-(void) showElementCollisionView;

-(void) sceneWasRendered;

@end
