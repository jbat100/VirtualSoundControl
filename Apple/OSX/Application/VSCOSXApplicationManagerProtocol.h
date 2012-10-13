//
//  VSCOSXApplicationManagerProtocol.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/13/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCMIDI.h"
#include "VSCMIDIOutputManager.h"
#include "VSCOBApplication.h"

@class VSCOBOSXSceneWindow;
@class VSCOBOSXSceneView;

@protocol VSCOSXApplicationManagerProtocol <NSObject>

@property(nonatomic, assign) VSC::MIDI::OutputManager::SPtr midiOutputManager;
@property(nonatomic, assign) VSC::OB::Application::SPtr ogreBulletApplication;

-(void) startOgreRendering;
-(void) stopOgreRendering;

@end
