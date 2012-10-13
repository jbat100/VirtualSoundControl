//
//  VSCApplicationManager.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/13/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCApplicationManagerProtocol.h"
#import "VSCOBOSXSceneWindow.h"
#import "VSCOSXMIDIWindowController.h"

#include "OgrePrerequisites.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"


@interface VSCApplicationManager : NSObject <VSCApplicationManagerProtocol>

@property (nonatomic, assign) VSC::MIDI::OutputManager::SPtr midiOutputManager;
@property (nonatomic, assign) VSC::OB::Application::SPtr ogreBulletApplication;

@property (nonatomic, strong) IBOutlet 

@property (nonatomic, strong) VSCOBOSXSceneWindow *window;

@end
