//
//  VSCOBOSXSceneView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 6/22/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "OgreOSXCocoaView.h"

#include "VSCOBOSXInputAdapter.h"

#include "VSCOBSceneController.h"
#include "VSCOBDisplayController.h"


@interface VSCOBOSXSceneView : OgreView {
    
    
}

@property (nonatomic, assign) VSC::OB::OSXInputAdapter::SPtr inputAdapter;

@end
