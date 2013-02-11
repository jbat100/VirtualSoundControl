//
//  VSCOBOSXSceneDisplayView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 6/22/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "OgreOSXCocoaView.h"

#import "VSCOBOSXSceneController.h"

#include "VSCOBKeyBindings.h"
#include "VSCOBDisplay.h"
#include "VSCOBInterface.h"
#include "VSCOBSceneController.h"
#include "VSCOBDisplayController.h"

@interface VSCOBOSXSceneDisplayView : OgreView

@property (nonatomic, assign) VSC::OB::InterfaceResponderChain::SPtr interfaceResponderChain;
@property (readonly, nonatomic, assign) VSC::OB::Display::WPtr display;

@property (weak) IBOutlet id<VSCOBOSXSceneController> sceneController;

@property (nonatomic, assign) VSC::OB::SceneController::SPtr internalSceneController;
@property (nonatomic, assign) VSC::OB::DisplayController::SPtr displayController;

// called by controller when the scene changes
-(void) setupControllerChain;

@end
