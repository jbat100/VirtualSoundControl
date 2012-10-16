//
//  VSCAppDelegate.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//


#import "VSCAppDelegate.h"
#import "VSCOSXApplicationManager.h"
#import "VSCOBOSXSceneWindowController.h"
#import "VSCOSXMIDIWindowController.h"

#import "VSCOBScene.h"
#import "VSCOBApplication.h"
#import "VSCOBPrimitivesDemo.h"
#import "VSCOBTriMeshDemo.h"
#import "VSCOBConstraintsDemo.h"

#import "VSCMIDI.h"
#import "VSCMIDIOutput.h"
#import "VSCMIDIOutputManager.h"
#import "VSCException.h"

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
    BOOST_ASSERT(self.applicationManager.ogreBulletSceneWindowController.ogreBulletSceneView);
    
    self.applicationManager.midiOutputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(self.applicationManager.midiOutputManager);
    
    
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
