//
//  VSCAppDelegate.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//


#import "VSCAppDelegate.h"
#import "VSCOSXApplicationManager.h"
#import "VSCOSXEnvironmentWindowController.h"
#import "VSCOSXMIDIWindowController.h"

#include "VSCOBScene.h"
#include "VSCOBApplication.h"
#include "VSCOBPrimitivesDemo.h"
#include "VSCOBTriMeshDemo.h"
#include "VSCOBConstraintsDemo.h"

#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"
#include "VSCMIDIOutputManager.h"
#include "VSCException.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

//#import "OgreOSXCocoaView.h"

@interface VSCAppDelegate ()

-(void) setupOgreBulletApplication;
-(void) teardownOgreBulletApplication;

@end

@implementation VSCAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    /*
     *  Create application manager, and setup midi output manager
     */
    
    self.applicationManager = [[VSCOSXApplicationManager alloc] init];
    BOOST_ASSERT(self.applicationManager);

    
    self.ogreBulletSceneWindowController = [[VSCOSXEnvironmentWindowController alloc] initWithWindowNibName:@"VSCOBOSXSceneWindow"];
    BOOST_ASSERT(self.ogreBulletSceneWindowController);
    self.ogreBulletSceneWindowController.applicationManager = self;
    
    NSWindow* w = self.ogreBulletSceneWindowController.window;
    NSLog(@"OgreBulletSceneWindow: %@", w);
    
    self.midiWindowController = [[VSCOSXMIDIWindowController alloc] initWithWindowNibName:@"VSCOSXMIDIWindow"];
    BOOST_ASSERT(self.midiWindowController);
    self.midiWindowController.applicationManager = self;
    
    
    /*
     *  Refresh outputs and open them all
     */
    
    self.applicationManager.midiOutputManager->refreshOutputs();
    
    const VSC::MIDI::Outputs& outputs = self.applicationManager.midiOutputManager->getOutputs();
    BOOST_FOREACH(VSC::MIDI::Output::SPtr output, outputs)
    {
        try {
            output->open();
        } catch (VSCMIDIException& e) {
            std::cerr << e.what() << " - Additional info: " << e.getValueForKey(VSCBaseExceptionAdditionalInfoKey) << std::endl;
        }
    }
    
    [self setupOgreBulletApplication];
    
    //[self showSceneWindow:nil];
    
    [self showMIDIWindow:nil];
    
    [self.applicationManager startOgreRendering];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification 
{
    [self.applicationManager stopOgreRendering];
    
    [self teardownOgreBulletApplication];
}

-(IBAction)showMIDIWindow:(id)sender
{
    [self.applicationManager.midiWindowController showWindow:sender];
}

-(IBAction)showSceneWindow:(id)sender
{
    VSC::OB::Scene::SPtr currentScene = self.applicationManager.ogreBulletApplication->getCurrentScene();
    
    self.applicationManager.ogreBulletSceneWindowController.ogreBulletScene = currentScene;
    
    [self.applicationManager.ogreBulletSceneWindowController showWindow:sender];
}


-(void) setupOgreBulletApplication {
    
    VSC::OB::Application::Scenes scenes;
    
    scenes.push_back(VSC::OB::Scene::SPtr(new VSC::OB::PrimitivesDemo()));
    scenes.push_back(VSC::OB::Scene::SPtr(new VSC::OB::TriMeshDemo()));
	scenes.push_back(VSC::OB::Scene::SPtr(new VSC::OB::ConstraintsDemo()));
    
    VSC::OB::Application::SPtr ogreBulletApplication = VSC::OB::Application::SPtr(new VSC::OB::Application(scenes));
    
    self.applicationManager.ogreBulletApplication = ogreBulletApplication; // this calls setupWithOgreView()
    
    //ogreBulletApplication->setupWithOgreView((__bridge void*)self.applicationManager.ogreBulletSceneWindowController.ogreBulletSceneView);
    
}

-(void) teardownOgreBulletApplication {
    
    /*
     *  Cleanup, setting the smart pointer to nil should take care of everything...
     */
    
    self.applicationManager.ogreBulletApplication = VSC::OB::Application::SPtr();
    
}


@end
