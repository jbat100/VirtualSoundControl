//
//  VSCApplicationManagerProtocol.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/13/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCMIDI.h"
#include "VSCMIDIOutputManager.h";

@class VSCOBOSXSceneWindow;
@class VSCOBOSXSceneView;

@protocol VSCApplicationManagerProtocol <NSObject>

@property(nonatomic, assign) VSC::MIDI::OutputManager::SPtr midiOutputManager;

@property (nonatomic, assign) IBOutlet VSCOBOSXSceneWindow *window;
@property (nonatomic, assign) IBOutlet VSCOBOSXSceneView *ogreView;

-(void) startOgreRendering;
-(void) stopOgreRendering;

@end
