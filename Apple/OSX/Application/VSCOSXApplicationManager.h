//
//  VSCOSXApplicationManager.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/13/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCOSXApplicationManagerProtocol.h"

#include "OgrePrerequisites.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"

@class VSCOBOSXSceneWindowController;
@class VSCOSXMIDIWindowController;

@interface VSCOSXApplicationManager : NSObject <VSCOSXApplicationManagerProtocol>

@property (nonatomic, assign) VSC::MIDI::OutputManager::SPtr midiOutputManager;
@property (nonatomic, assign) VSC::OB::Application::SPtr ogreBulletApplication;

@property (nonatomic, strong) VSCOBOSXSceneWindowController *ogreBulletSceneWindowController;
@property (nonatomic, strong) VSCOSXMIDIWindowController *midiWindowController;

@end
