//
//  VSCOBOSXSceneDisplayView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 6/22/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "OgreOSXCocoaView.h"

#include "VSCOBKeyBindings.h"
#include "VSCOBDisplay.h"
#include "VSCOBInterface.h"



@interface VSCOBOSXSceneDisplayView : OgreView 

@property (nonatomic, assign) VSC::OB::Display::WPtr display;
@property (nonatomic, assign) VSC::OB::InterfaceResponderChain::SPtr interfaceResponderChain;

@end
