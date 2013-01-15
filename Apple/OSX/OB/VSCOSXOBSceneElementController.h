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
#include "VSCOBElement.h"
#include "VSCIMEvent.h"

@protocol VSCIMOSXCollisionEventChainController;

/*
 *  Main controller for a Element
 */

@protocol VSCOSXOBSceneElementController <NSObject>

@property (nonatomic, assign) VSC::OB::Element_WPtr element;

@property (assign) id <VSCOSXEnvironmentController> environmentController;

-(void) showElementDetailView;
-(void) showElementCollisionView;

@end


