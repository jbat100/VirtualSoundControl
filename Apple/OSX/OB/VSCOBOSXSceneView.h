//
//  VSCOBOSXSceneView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 6/22/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "OgreOSXCocoaView.h"

#include "VSCOBKeyBindings.h"
#include "VSCOBScene.h"



@interface VSCOBOSXSceneView : OgreView 

@property (nonatomic, assign) VSC::OB::KeyBindings::SPtr keyBindings;

@property (nonatomic, assign) VSC::OB::Scene::WPtr scene;


@end
